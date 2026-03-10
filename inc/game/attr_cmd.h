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

#endif // __ATTR_IMPL_H__