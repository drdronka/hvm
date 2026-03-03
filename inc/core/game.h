#ifndef __GAME_H__
#define __GAME_H__

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_render.h>

#include "gcfg.h"

typedef struct game_context
{
  int win_x;
  int win_y;
  char* app_name;
  Uint64 tick_delta;
  SDL_Window *window;
  SDL_Renderer *renderer;
} game_context_t;

ret_e game_init();
ret_e game_update();
ret_e game_event(SDL_Event *event);
ret_e game_exit();
game_context_t *game_context_get();

#endif // __GAME_H__