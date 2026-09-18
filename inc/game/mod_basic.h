#pragma once

#include "core_def.h"
#include "unit.h"
#include "anim.h"

// ------------------------------------------------------------- //

typedef enum
{
  MOVE_TYPE_ABS = 0,
  MOVE_TYPE_REL
} move_type_e;

class mod_psyh_c : public mod_c
{
public:  
  float pos_x;
  float pos_y;
  float size_x;
  float size_y;
  float speed;
  float dir;

  mod_psyh_c(float pos_x, float pos_y, float size_x, float size_y, float speed, float dir);
  ~mod_psyh_c();
  ret_e move(float dst_x, float dst_y, move_type_e type, bool temporary);
  void pos_rel_to_abs(float *x, float *y);
};

// ------------------------------------------------------------- //

class mod_visu_c : public mod_c
{
public:
  anim_c *anim;
  const char *anim_stage_name;
  bool anim_cycle;
  Uint32 anim_ticks;
  bool visible;

  mod_visu_c(anim_c *anim, const char *stage_name, bool cycle);
  ~mod_visu_c();
  void run();
  void anim_stage_set(const char *stage_name, bool cycle, bool reset);
  Uint32 anim_ticks_get(const char *stage_name);
};

// ------------------------------------------------------------- //

class mod_wander_c : public mod_c
{
public:
  bool initialized;
  float range;
  Uint32 ticks;
  Uint32 ticks_next;
  Uint32 ticks_max;
  float org_x;
  float org_y;

  mod_wander_c(float range, Uint32 ticks_max);
  ~mod_wander_c();
  void run();
  void rebase();
};
