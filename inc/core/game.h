#ifndef __GAME_H__
#define __GAME_H__

#include <SDL3/SDL_init.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_render.h>

#include "gcfg.h"
#include "list.h"

typedef struct game_context
{
  int win_x;
  int win_y;
  char* app_name;
  Uint64 ticks_delta_ms;
  Uint64 ticks_total_ms;
  SDL_Window *window;
  SDL_Renderer *renderer;
  list_t *unit_list;
} game_context_t;

SDL_AppResult game_init();
SDL_AppResult game_update();
SDL_AppResult game_event(SDL_Event *event);
void game_exit();
game_context_t *game_context_get();

#endif // __GAME_H__