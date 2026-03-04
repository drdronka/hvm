#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include <SDL3/SDL_render.h>

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

void game_context_init();
game_context_t *game_context_get();

#endif 