#include <SDL3/SDL_init.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_hints.h>

#include "gcfg.h"
#include "game.h"
#include "unit.h"
#include "list.h"
#include "attr.h"
#include "psyh.h"
#include "visu.h"
#include "drift.h"

SDL_Surface *sample_surf;
SDL_Texture *sample_tex;

unit_t *unit_head = NULL;

Uint64 ticks_total = 0;
Uint64 last_spawn = 0;

game_context_t context;

ret_e assets_load()
{
  //sample_surf = SDL_LoadPNG("./img/black_square.png");
  sample_surf = SDL_LoadPNG("./assets/img/cage.png");
  if(!sample_surf)
  {
    SDL_Log("error: %s", SDL_GetError());
    return RET_ERR;
  }

  sample_tex = SDL_CreateTextureFromSurface(context.renderer, sample_surf);
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

  visu = visu_new();
  visu->visible = 1;
  visu->tex = sample_tex;
  visu->renderer = context.renderer;
  attr = unit_attr_add(unit);
  attr->type = ATTR_VISU;
  attr->data = (void*)visu;
  attr->func = visu_func;

  attr = unit_attr_add(unit);
  attr->type = ATTR_DRIFT;
  attr->func = drift_func;
}

void deinit()
{
  SDL_Log("game: deinitializing");

  if(sample_tex) SDL_DestroyTexture(sample_tex);
  if(sample_surf) SDL_DestroySurface(sample_surf);
  if(context.window) SDL_DestroyWindow(context.window);

  SDL_Log("game: deinitialization finished");
}

// ===================== GLOBAL FUNC ===================== //

SDL_AppResult game_init()
{
  SDL_Log("%s", APPNAME);
  
  context.win_x = WINX;
  context.win_y = WINY;
  context.app_name = APPNAME;
  context.window = NULL;
  context.renderer = NULL;

  SDL_Log("game: fps limit: %s", FPS_LIMIT);
  SDL_SetHint(SDL_HINT_MAIN_CALLBACK_RATE, FPS_LIMIT);

  SDL_Log("game: initializing SDL");
  if(!SDL_Init(SDL_INIT_VIDEO))
  {
    SDL_Log("error: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  SDL_Log("game: creating window x[%d] y[%d]", context.win_x, context.win_y);
  if(!SDL_CreateWindowAndRenderer(
    APPNAME, 
    context.win_x, 
    context.win_y, 0, 
    &context.window, 
    &context.renderer))
  {
    SDL_Log("game: error: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  SDL_Log("game: loading assets");
  if(!assets_load())
  {
    SDL_Log("game: error: failed to load assets");
    return SDL_APP_FAILURE;
  }

  SDL_Log("game: initialization finished");

  return SDL_APP_CONTINUE;
}

SDL_AppResult game_update()
{
  #if STEP_MODE
    SDL_Log("game: update");
  #endif

  Uint64 ticks = SDL_GetTicksNS();
  if(ticks_total == 0)
  {
    ticks_total = ticks;
    return SDL_APP_CONTINUE;
  }
  context.tick_delta = ticks - ticks_total;
  ticks_total = ticks;

  if(last_spawn + 1000000000 < ticks_total)
  {
    spawn_sample();
    last_spawn = ticks_total;
  }

  SDL_SetRenderDrawColor(context.renderer, 0, 200, 200, 0);
  SDL_RenderFillRect(context.renderer, NULL);

  unit_t *unit = unit_head;
  while(unit)
  {
    unit_proc(unit);
    unit = unit->next;
  }

  SDL_RenderPresent(context.renderer);

  #if STEP_MODE
    SDL_Delay(1000);
  #endif

  return SDL_APP_CONTINUE;
}

SDL_AppResult game_event(SDL_Event *event)
{
  Uint8 exit = 0;

  switch(event->type)
  {
    case SDL_EVENT_KEY_DOWN:
      const bool *keys = SDL_GetKeyboardState(NULL);

      if(keys[SDL_SCANCODE_ESCAPE])
        exit = 1;

      //if(keys[SDL_SCANCODE_UP])    if(ypos >= CHAR_SIZE) ypos -= CHAR_SIZE;
      //if(keys[SDL_SCANCODE_DOWN])  if(ypos < WINY - CHAR_SIZE) ypos += CHAR_SIZE;
      //if(keys[SDL_SCANCODE_LEFT])  if(xpos >= CHAR_SIZE) xpos -= CHAR_SIZE;
      //if(keys[SDL_SCANCODE_RIGHT]) if(xpos < WINX - CHAR_SIZE) xpos += CHAR_SIZE;
      break;

    case SDL_EVENT_QUIT:
      exit = 1;
      break;
  }

  if(exit)
  {
    SDL_Quit();
    return SDL_APP_SUCCESS;
  }

  return SDL_APP_CONTINUE;
}

void game_exit()
{
  deinit();
}

game_context_t *game_context_get()
{
  return &context;
}