#pragma once

#include <SDL3/SDL_events.h>

#include "game.h"

class game_c;

class gui_c
{
public:  
  game_c *game;

  Uint8 sel_en;
  float sel_x;
  float sel_y; 

  /* singleton */
  static gui_c *get();

  SDL_AppResult init();
  SDL_AppResult event(SDL_Event *event);
  void render_back();
  void render_front();
  void draw_sel_rect(float pos_x, float pos_y, float size_x, float size_y);

private:
  void sel_start(float pos_x, float pos_y);
  void sel_finish(float pos_x, float pos_y);
  void sel_finish_mult(float pos_x, float pos_y);
  void sel_finish_single(float mouse_x, float mouse_y);
};