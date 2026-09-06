#pragma once

#include <SDL3/SDL_render.h>

#include "core_def.h"
#include "unit.h"
#include "asset.h"
#include "anim.h"
#include "unit.h"

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
  //void run();
  //void clean();
  ret_e move(float dst_x, float dst_y, move_type_e type, bool temporary);
  void pos_get(float *x, float *y);
  void pos_rel_to_abs(float *x, float *y);
};

// ------------------------------------------------------------- //

class mod_visu_c : public mod_c
{
public:
  anim_c *anim;
  anim_stage_id_e anim_stage_id;
  Uint8 anim_cycle;
  Uint32 anim_ticks;
  bool visible;

  mod_visu_c(anim_c *anim, anim_stage_id_e stage_id);
  ~mod_visu_c();
  void run();
  //void clean();
  void anim_stage_set(anim_stage_id_e stage_id, bool cycle, bool reset);
  Uint32 anim_ticks_get(anim_stage_id_e stage_id);
};

// ------------------------------------------------------------- //

#if 0
typedef struct attr_wander_data
{
  Uint8 initialized;
  float range;
  Uint32 ticks_ms;
  Uint32 ticks_next_ms;
  Uint32 ticks_max_ms;
  float org_x;
  float org_y;
} attr_wander_data_t;

attr_t *attr_wander_new(float range, float ticks_max_ms);
void attr_wander_run(void *unit_ref, void *attr_ref);
void attr_wander_pos_update(attr_wander_data_t *data, float pos_x, float pos_y);
#endif

