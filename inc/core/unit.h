#ifndef __UNIT_H__
#define __UNIT_H__

#include <SDL3/SDL_stdinc.h>

#include "attr.h"
#include "list.h"
#include "unit_def.h"

typedef struct unit
{
  Uint8 selected;
  unit_id_e id;
  list_t *attr_list;
} unit_t;

unit_t *unit_new(unit_id_e unit_id);
void unit_del(unit_t *unit);
void unit_attr_add(unit_t *unit, attr_t *attr);
void unit_attr_add_head(unit_t *unit, attr_t *attr);
void unit_attr_clean(unit_t *unit, attr_t *attr);
void unit_list_destroy(list_t *list);
void *unit_attr_data_get(unit_t *unit, Uint32 id);
void *unit_cmd_clear_all(unit_t *unit);
Uint8 unit_cmd_is_empty(unit_t *unit);

#endif // __UNIT_H__