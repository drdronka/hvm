#pragma once

#include "game_ctx.h"

class gui_c
{
public:  
  game_ctx_c *ctx;

  gui_c();
  ~gui_c();
  void bg_draw();
  void draw_sel_rect(float pos_x, float pos_y, float size_x, float size_y);

  static gui_c* get() 
  {
    static gui_c instance;
    return &instance;
  }
};