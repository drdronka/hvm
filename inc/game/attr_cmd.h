#ifndef __ATTR_CMD_H__
#define __ATTR_CMD_H__

#include <SDL3/SDL_render.h>

#include "attr.h"
#include "attr_def.h"
#include "attr_basic.h"

// ------------------------------------------------------------- //

typedef struct attr_move_data
{
  float dst_x;
  float dst_y;
  move_type_e type;
  Uint8 temporary;
  Uint8 initialized;
} attr_move_data_t;

attr_t *attr_move_new(float dst_x, float dst_y, move_type_e type, Uint8 temporary);
void attr_move_run(void *unit_ref, void *attr_ref);
void attr_move_clean(void *unit_ref, void *attr_ref);

// ------------------------------------------------------------- //

typedef struct attr_death_data
{
  Uint32 ticks_ms;
  Uint32 ticks_limit_ms;
} attr_death_data_t;

attr_t *attr_death_new();
void attr_death_run(void *unit_ref, void *attr_ref);
void attr_death_clean(void *unit_ref, void *attr_ref);

// ------------------------------------------------------------- //

typedef struct attr_enter_data
{
  Uint32 ticks_ms;
  Uint32 ticks_limit_ms;
} attr_enter_data_t;

attr_t *attr_enter_new();
void attr_enter_run(void *unit_ref, void *attr_ref);
void attr_enter_clean(void *unit_ref, void *attr_ref);

// ------------------------------------------------------------- //

#endif // __ATTR_IMPL_H__