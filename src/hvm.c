#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_render.h>

#include "unit.h"
#include "gcfg.h"

SDL_Window *window;
SDL_Renderer *renderer;

SDL_Surface *sample_surf;
SDL_Texture *sample_tex;

unit_t *unit_head = NULL;

Uint64 ticks_total = 0;
Uint64 last_spawn = 0;
Uint64 g_delta = 0;

ret_e assets_load()
{
  //sample_surf = SDL_LoadPNG("./img/black_square.png");
  sample_surf = SDL_LoadPNG("./img/cage.png");
  if(!sample_surf)
  {
    SDL_Log("error: %s", SDL_GetError());
    return RET_ERR;
  }

  sample_tex = SDL_CreateTextureFromSurface(renderer, sample_surf);
  if(!sample_tex)
  {
    SDL_Log("error: %s", SDL_GetError());
    return RET_ERR;
  }

  return RET_OK;
}

void spawn_sample()
{
  unit_t *unit;
  attr_t *attr;
  psyh_t *psyh;
  visu_t *visu;
  behv_t *behv;

  unit = unit_add(&unit_head);
  unit->active = 1;

  psyh = psyh_new();
  psyh->pos_x = WINX / 2;
  psyh->pos_y = WINY / 2;
  psyh->vel_x = 1;
  psyh->vel_y = 1;
  psyh->size_x = 128;
  psyh->size_y = 128;
  attr = unit_attr_add(unit);
  attr->type = ATTR_PSYH;
  attr->data = (void*)psyh;

  behv = behv_new();
  behv->f = behv_drift;
  attr = unit_attr_add(unit);
  attr->type = ATTR_BEHV;
  attr->data = (void*)behv;

  visu = visu_new();
  visu->visible = 1;
  visu->tex = sample_tex;
  visu->renderer = renderer;
  attr = unit_attr_add(unit);
  attr->type = ATTR_VISU;
  attr->data = (void*)visu;

  behv = behv_new();
  behv->f = behv_draw;
  attr = unit_attr_add(unit);
  attr->type = ATTR_BEHV;
  attr->data = (void*)behv;
}

ret_e game_init()
{

}

void deinit()
{
  if(sample_tex) SDL_DestroyTexture(sample_tex);
  if(sample_surf) SDL_DestroySurface(sample_surf);
  if(window) SDL_DestroyWindow(window);
}

SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv)
{
  SDL_Log("%s", APPNAME);

  window = NULL;
  renderer = NULL;

  if(!SDL_Init(SDL_INIT_VIDEO))
  {
    SDL_Log("error: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  if(!SDL_CreateWindowAndRenderer(APPNAME, WINX, WINY, 0, &window, &renderer))
  {
    SDL_Log("error: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  if(!assets_load())
  {
    SDL_Log("error: failed to load assets");
    return SDL_APP_FAILURE;
  }

  game_init();

  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
  Uint64 ticks = SDL_GetTicksNS();
  if(ticks_total == 0)
  {
    ticks_total = ticks;
    return SDL_APP_CONTINUE;
  }
  g_delta = ticks - ticks_total;
  ticks_total = ticks;

  if(last_spawn + 1000000000 < ticks_total)
  {
    spawn_sample();
    last_spawn = ticks_total;
  }

  SDL_SetRenderDrawColor(renderer, 0, 200, 200, 0);
  SDL_RenderFillRect(renderer, NULL);

  unit_t *unit = unit_head;
  while(unit)
  {
    unit_proc(unit);
    unit = unit->next;
  }

  SDL_RenderPresent(renderer);

  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
  switch(event->type)
  {
    case SDL_EVENT_KEY_DOWN:
      const bool *keys = SDL_GetKeyboardState(NULL);
      //if(keys[SDL_SCANCODE_UP])    if(ypos >= CHAR_SIZE) ypos -= CHAR_SIZE;
      //if(keys[SDL_SCANCODE_DOWN])  if(ypos < WINY - CHAR_SIZE) ypos += CHAR_SIZE;
      //if(keys[SDL_SCANCODE_LEFT])  if(xpos >= CHAR_SIZE) xpos -= CHAR_SIZE;
      //if(keys[SDL_SCANCODE_RIGHT]) if(xpos < WINX - CHAR_SIZE) xpos += CHAR_SIZE;
      break;
    case SDL_EVENT_QUIT:
      deinit();
      SDL_Quit();
      break;
  }

  return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
  deinit();
}
