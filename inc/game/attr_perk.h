#ifndef __ATTR_IMPL_H__
#define __ATTR_IMPL_H__

#include <SDL3/SDL_render.h>

#include "attr.h"
#include "attr_def.h"
#include "asset.h"
#include "anim.h"

// ------------------------------------------------------------- //

typedef struct attr_psyh_data
{
  float pos_x;
  float pos_y;
  float size_x;
  float size_y;
  float speed;
  float dir;
  float dst_x;
  float dst_y;
} attr_psyh_data_t;

attr_t *attr_psyh_new(float pos_x, float pos_y, float size_x, float size_y, float speed);

// ------------------------------------------------------------- //

typedef struct attr_visu_data
{
  anim_t *anim;
  Uint8 anim_stage_id;
  Uint32 anim_ticks_ms;
  Uint8 visible;
} attr_visu_data_t;

attr_t *attr_visu_new(anim_t *anim, Uint8 anim_stage_id, Uint8 visible);

// ------------------------------------------------------------- //

#endif // __ATTR_IMPL_H__