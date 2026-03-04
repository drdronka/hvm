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
#include "asset.h"

#include <stdio.h>

Uint64 last_spawn = 0;

game_context_t context;

void spawn_sample()
{
  SDL_Log("spawning sample");

  unit_t *unit = unit_new(1);
  unit_attr_add(unit, psyh_new(context.win_x / 2, context.win_y / 2, 1, 1, 128, 128));
  unit_attr_add(unit, visu_new(asset_texture_get("cage"), 1));
  unit_attr_add(unit, drift_new());
  list_add(context.unit_list, (void*)unit);

  attr_t *attr;
  list_node_t *iter = list_iter_init(unit->attr_list);
  while(attr = list_iter_next(&iter))
  {
    fprintf(stderr, "attr type [%d]\n", attr->type);
  }
}

void deinit()
{
  SDL_Log("game: deinitializing");

  asset_texture_free_all();
  if(context.renderer) SDL_DestroyRenderer(context.renderer);
  if(context.window) SDL_DestroyWindow(context.window);

  SDL_Log("game: deinitialization finished");
}

// ===================== GLOBAL FUNC ===================== //

SDL_AppResult game_init()
{
  SDL_Log("%s", APPNAME);
  
  memset(&context, 0, sizeof(context));
  context.win_x = WINX;
  context.win_y = WINY;
  context.app_name = APPNAME;
  context.unit_list = list_new();
  
  char *fps_limit;
  #if STEP_MODE  
    fps_limit = "1";
  #else
    fps_limit = FPS_LIMIT;
  #endif

  SDL_Log("game: fps limit: %s", fps_limit);
  SDL_SetHint(SDL_HINT_MAIN_CALLBACK_RATE, fps_limit);

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
  if(!asset_texture_load_all())
  {
    SDL_Log("game: error: failed to load assets");
    return SDL_APP_FAILURE;
  }

  SDL_Log("game: initialization finished");

  return SDL_APP_CONTINUE;
}

SDL_AppResult game_update()
{
  Uint64 ticks_ms = SDL_GetTicksNS() / 1000;
  if(context.ticks_total_ms == 0)
  {
    context.ticks_total_ms = ticks_ms;
    return SDL_APP_CONTINUE;
  }
  context.ticks_delta_ms = ticks_ms - context.ticks_total_ms;
  context.ticks_total_ms = ticks_ms;

  if(last_spawn + 1000000 < ticks_ms)
  {
    spawn_sample();
    last_spawn = context.ticks_total_ms;
  }

  SDL_SetRenderDrawColor(context.renderer, 0, 200, 200, 0);
  SDL_RenderFillRect(context.renderer, NULL);  

  unit_t *unit;
  list_node_t *unit_iter = list_iter_init(context.unit_list);
  while(unit = list_iter_next(&unit_iter))
  {
    attr_t *attr;
    list_node_t *attr_iter = list_iter_init(unit->attr_list);
    while(attr = list_iter_next(&attr_iter))
      if(attr->run) 
        attr->run((void*)unit);
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