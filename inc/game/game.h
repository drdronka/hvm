#pragma once

#include <vector>

#include <SDL3/SDL_init.h>
#include <SDL3/SDL_render.h>

#include "unit.h"
#include "asset.h"
#include "anim.h"
#include "gui.h"
#include "util.h"

class gui_c;

class game_c
{
public:
  gui_c *gui;

  /* window */
  int win_x;
  int win_y;
  const char* app_name;

  /* time */
  Uint64 ticks_delta_ms;
  Uint64 ticks_total_ms;
  float move_mult;

  /* SDL */
  SDL_Window *window;
  SDL_Renderer *renderer;

  /* engine */
  std::vector<unit_c*> units;
  std::vector<asset_tex_c*> textures;
  std::vector<anim_c*> anims;

  /* singleton */
  static game_c *get();

  SDL_AppResult init();
  void deinit();
  SDL_AppResult update();

  void units_move(float dst_x, float dst_y, bool clear_cmd_queue);
  void units_kill(bool clear_cmd_queue);
  void worm_spawn(float pos_x, float pos_y); 

private:
  ret_e assets_load();
  void ticks_update();
};


