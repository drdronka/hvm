#ifndef __PSYH_H__
#define __PSYH_H__

#include "attr.h"

typedef struct psyh_data
{
  float pos_x;
  float pos_y;
  float vel_x;
  float vel_y;
  float size_x;
  float size_y;
} psyh_data_t;

attr_t *psyh_new(float pos_x, float pos_y, float vel_x, float vel_y, float size_x, float size_y);

#endif // __PSYH_H__