#pragma once

#include <vector>
#include <SDL3/SDL_render.h>

#include "util.h"

// ------------------------------------------------------------- //

class anim_step_c
{
public:
  SDL_Texture *tex;
  Uint32 ticks_ms;

  anim_step_c(SDL_Texture *tex, Uint32 ticks_ms);
  ~anim_step_c();
  ret_e verify();
};

// ------------------------------------------------------------- //

class anim_stage_c
{
public:
  char *name;
  std::vector<anim_step_c*> steps;
  Uint32 ticks_total_ms;

  anim_stage_c(const char *name);
  ~anim_stage_c();
  void step_add(anim_step_c *step);
  ret_e verify();
};

// ------------------------------------------------------------- //

class anim_c
{
public:
  char *name;
  std::vector<anim_stage_c*> stages;

  anim_c(const char *name);
  ~anim_c();
  void stage_add(anim_stage_c *stage);
  ret_e verify();
  SDL_Texture *tex_get(const char *stage_name, Uint32 *ticks_ms, bool cycle);
  Uint32 ticks_get(const char *stage_name);
};

anim_c *anim_get(std::vector<anim_c*> anims, const char *name);
