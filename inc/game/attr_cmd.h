#ifndef __ATTR_CMD_H__
#define __ATTR_CMD_H__

#include <SDL3/SDL_render.h>

#include "attr.h"
#include "attr_def.h"

// ------------------------------------------------------------- //

typedef struct attr_move_data
{
  float dst_x;
  float dst_y;
  Uint8 initialized;
} attr_move_data_t;

attr_t *attr_move_new(float dst_x, float dst_y);

// ------------------------------------------------------------- //

#endif // __ATTR_IMPL_H__