#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include <SDL3/SDL_render.h>

#include "list.h"
#include <anim.h>
#include <vector>

typedef struct game_ctx
{
  // window
  int win_x;
  int win_y;
  const char* app_name;

  // time
  Uint64 ticks_delta_ms;
  Uint64 ticks_total_ms;
  float move_mult;

  // SDL
  SDL_Window *window;
  SDL_Renderer *renderer;

  // engine
  list_t *unit_list;
  list_t *tex_list;
  std::vector<anim_obj*> anims;

  // gui - move to separate module
  Uint8 sel_en;
  float sel_x;
  float sel_y; 

} game_ctx_t;

void game_ctx_init();
game_ctx_t *game_ctx_get();

void game_ctx_color_set_background();
void game_ctx_color_set_select();

#endif 