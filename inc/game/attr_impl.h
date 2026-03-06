#ifndef __ATTR_IMPL_H__
#define __ATTR_IMPL_H__

#include <SDL3/SDL_render.h>

#include "attr.h"

typedef enum 
{
  ATTR_NONE = 0,
  ATTR_PSYH,
  ATTR_VISU, 
  ATTR_DRIFT,
} attr_type_e;

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
  Uint8 moving;
} attr_psyh_data_t;

attr_t *attr_psyh_new(float pos_x, float pos_y, float size_x, float size_y, float speed);

// ------------------------------------------------------------- //

typedef struct attr_visu_data
{  
  SDL_Texture *tex;
  Uint8 visible;
} attr_visu_data_t;

attr_t *attr_visu_new(SDL_Texture *tex, Uint8 visible);

// ------------------------------------------------------------- //

attr_t *attr_drift_new();

// ------------------------------------------------------------- //

#endif // __ATTR_IMPL_H__