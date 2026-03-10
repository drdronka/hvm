#ifndef __ATTR_IMPL_H__
#define __ATTR_IMPL_H__

#include <SDL3/SDL_render.h>

#include "attr.h"
#include "attr_def.h"
#include "asset.h"
#include "anim.h"
#include "unit.h"

// ------------------------------------------------------------- //

typedef enum
{
  MOVE_TYPE_ABS = 0,
  MOVE_TYPE_REL
} move_type_e;

typedef struct attr_psyh_data
{
  float pos_x;
  float pos_y;
  float size_x;
  float size_y;
  float speed;
  float dir;
} attr_psyh_data_t;

attr_t *attr_psyh_new(float pos_x, float pos_y, float size_x, float size_y, float speed, float dir);
ret_e attr_psyh_move(unit_t *unit, float dst_x, float dst_y, move_type_e type, Uint8 temporary);
void attr_psyh_rel_to_abs(attr_psyh_data_t *data, float *x, float *y);
void attr_psyh_pos_get(attr_psyh_data_t *data, float *x, float *y);

// ------------------------------------------------------------- //

typedef struct attr_visu_data
{
  anim_t *anim;
  Uint8 anim_stage_id;
  Uint32 anim_ticks_ms;
  Uint8 visible;
} attr_visu_data_t;

attr_t *attr_visu_new(anim_t *anim, anim_stage_id_e stage_id);
void attr_visu_run(void *unit_ref, void *attr_ref);
void attr_visu_anim_stage_set(attr_visu_data_t *data, anim_stage_id_e stage_id);

// ------------------------------------------------------------- //

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

// ------------------------------------------------------------- //

#endif // __ATTR_IMPL_H__