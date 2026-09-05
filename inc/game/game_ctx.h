#pragma once

#include <SDL3/SDL_render.h>

#include "anim.h"
#include "asset.h"
#include "unit.h"
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
  std::vector<unit_c*> units;
  std::vector<asset_tex_c*> textures;
  std::vector<anim_c*> anims;

  // gui - move to separate module
  Uint8 sel_en;
  float sel_x;
  float sel_y; 

} game_ctx_t;

void game_ctx_init();
game_ctx_t *game_ctx_get();