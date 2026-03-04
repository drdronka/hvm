#include <malloc.h>
#include <string.h>

#include "psyh.h"

/// GLOBAL FUNC ///

attr_t *psyh_new(float pos_x, float pos_y, float vel_x, float vel_y, float size_x, float size_y)
{
  psyh_data_t *psyh_data = (psyh_data_t*)malloc(sizeof(psyh_data_t));
  memset(psyh_data, 0, sizeof(psyh_data_t));
  psyh_data->pos_x = pos_x;
  psyh_data->pos_y = pos_y;
  psyh_data->vel_x = vel_x;
  psyh_data->vel_y = vel_y;
  psyh_data->size_x = size_x;
  psyh_data->size_y = size_y;
  return attr_new(ATTR_PSYH, psyh_data, NULL);
}
