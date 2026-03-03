#ifndef __UNIT_H__
#define __UNIT_H__

#include <SDL3/SDL_render.h>

#include "attr.h"

typedef struct unit
{
  Uint8 active;
  attr_t *attr_head;
  struct unit *next;
} unit_t;

unit_t *unit_add(unit_t **head);
void unit_del(unit_t **head, unit_t *unit);
void unit_proc(unit_t *unit);
attr_t *unit_attr_get(unit_t *unit, attr_type_e type);
attr_t *unit_attr_add(unit_t *unit);

#endif // __UNIT_H__