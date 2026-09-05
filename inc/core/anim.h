#pragma once

#include "SDL3/SDL_render.h"

#include "util.h"
#include <vector>

typedef enum
{
  ANIM_STAGE_ID_IDLE = 0,
  ANIM_STAGE_ID_ENTER,
  ANIM_STAGE_ID_MOVE,
  ANIM_STAGE_ID_DEATH,
} anim_stage_id_e;

class anim_step_c
{
public:
  SDL_Texture *tex;
  Uint32 ticks_ms;

  anim_step_c(SDL_Texture *tex, Uint32 ticks_ms);
  ~anim_step_c();
  ret_e verify();
};

class anim_stage_c
{
public:
  anim_stage_id_e id;
  std::vector<anim_step_c*> steps;
  Uint32 ticks_total_ms;

  anim_stage_c(anim_stage_id_e stage_id);
  ~anim_stage_c();
  void step_add(anim_step_c *step);
  ret_e verify();
};

class anim_c
{
public:
  char *name;
  std::vector<anim_stage_c*> stages;

  anim_c(const char *name);
  ~anim_c();
  void stage_add(anim_stage_c *stage);
  ret_e verify();
  SDL_Texture *tex_get(anim_stage_id_e stage_id, Uint32 *ticks_ms, bool cycle);
  Uint32 ticks_get(anim_stage_id_e stage_id);
};

anim_c *anim_get(std::vector<anim_c*> anims, const char *name);
