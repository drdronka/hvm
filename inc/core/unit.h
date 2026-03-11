#ifndef __UNIT_H__
#define __UNIT_H__

#include <SDL3/SDL_stdinc.h>

#include "attr.h"
#include "list.h"
#include "unit_def.h"

typedef struct unit
{
  Uint8 selected;
  Uint8 dead;
  unit_id_e id;
  list_t *attr_list;
} unit_t;

// create / destroy
unit_t *unit_new(unit_id_e unit_id);
void unit_del(unit_t *unit);
void unit_attr_add(unit_t *unit, attr_t *attr);
void unit_attr_add_head(unit_t *unit, attr_t *attr);
void unit_list_destroy(list_t *list);

// batch run / clean
void unit_attr_run(unit_t *unit, attr_id_e id, attr_type_e type);
void unit_attr_clean(unit_t *unit, attr_id_e id, attr_type_e type);
void unit_list_attr_run(list_t *list, attr_id_e id, attr_type_e type);
void unit_list_attr_clean(list_t *list, attr_id_e id, attr_type_e type);
void unit_list_remove_dead(list_t *list);

// attr access
void *unit_attr_data_get(unit_t *unit, Uint32 id);

// command queue
void *unit_cmd_clear_all(unit_t *unit);
Uint8 unit_cmd_is_empty(unit_t *unit);

#endif // __UNIT_H__