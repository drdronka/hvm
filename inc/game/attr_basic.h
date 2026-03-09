#ifndef __ATTR_IMPL_H__
#define __ATTR_IMPL_H__

#include <SDL3/SDL_render.h>

#include "attr.h"
#include "attr_def.h"
#include "asset.h"
#include "anim.h"

// ------------------------------------------------------------- //

typedef enum 
{
  PSYH_MOVE_ONGOING = 0,
  PSYH_MOVE_FINISHED,
} psyh_move_ret;

typedef struct attr_psyh_data
{
  float pos_x;
  float pos_y;
  float size_x;
  float size_y;
  float speed;
  float dir;
} attr_psyh_data_t;

attr_t *attr_psyh_new(float pos_x, float pos_y, float size_x, float size_y, float speed);
void attr_psyh_dir_set(attr_psyh_data_t *data, float dst_x, float dst_y);
Uint8 attr_psyh_move(attr_psyh_data_t *data, float dst_x, float dst_y);

// ------------------------------------------------------------- //

typedef struct attr_visu_data
{
  anim_t *anim;
  Uint8 anim_stage_id;
  Uint32 anim_ticks_ms;
  Uint8 visible;
} attr_visu_data_t;

attr_t *attr_visu_new(anim_t *anim, Uint8 anim_stage_id, Uint8 visible);
void attr_visu_anim_stage_set(attr_visu_data_t *data);

// ------------------------------------------------------------- //

#endif // __ATTR_IMPL_H__