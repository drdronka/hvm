#pragma once

#include <SDL3/SDL_init.h>
#include <SDL3/SDL_events.h>

#include "game_ctx.h"

class game_c
{
public:
  game_ctx_c *ctx;

  game_c();
  ~game_c();

  SDL_AppResult init();
  SDL_AppResult update();
  SDL_AppResult event(SDL_Event *event);
  void exit();

private:
  ret_e assets_load();
  void ticks_update();
  void deinit();
  void sel_start(float pos_x, float pos_y);
  void sel_mult_finish(float pos_x, float pos_y);
  void sel_single(float mouse_x, float mouse_y);
  void sel_finish(float pos_x, float pos_y);
  void units_move(float dst_x, float dst_y, bool clear_cmd_queue);
  void units_kill(bool clear_cmd_queue);
  void worm_spawn(float pos_x, float pos_y);
};


