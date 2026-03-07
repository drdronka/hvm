#ifndef __UNIT_H__
#define __UNIT_H__

#include <SDL3/SDL_stdinc.h>

#include "attr.h"
#include "list.h"

typedef struct unit
{
  Uint8 active;
  Uint8 selected;
  list_t *attr_list;
} unit_t;

unit_t *unit_new(Uint8 active);
void unit_attr_add(unit_t *unit, attr_t *attr);
void unit_attr_add_head(unit_t *unit, attr_t *attr);
void unit_attr_del(unit_t *unit, attr_t *attr);
void *unit_attr_data_get(unit_t *unit, Uint32 type);
void *unit_cmd_clear_all(unit_t *unit);

#endif // __UNIT_H__