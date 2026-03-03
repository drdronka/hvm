#ifndef __PSYH_H__
#define __PSYH_H__

#include "attr.h"

typedef struct psyh
{
  float pos_x;
  float pos_y;
  float vel_x;
  float vel_y;
  float size_x;
  float size_y;
} psyh_t;

psyh_t *psyh_new();

#endif // __PSYH_H__