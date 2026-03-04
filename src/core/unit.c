#include <malloc.h>

#include "log.h"
#include "gcfg.h"
#include "unit.h"
#include "attr.h"
#include "game.h"
#include "asset.h"

// ======================== LOCAL DATA ========================= //

// ======================== LOCAL FUNC ========================= //

// ======================== GLOBAL FUNC ======================== //

unit_t *unit_new(Uint8 active)
{
  unit_t *unit = malloc(sizeof(unit_t));
  memset(unit, 0, sizeof(unit_t));
  unit->attr_list = list_new();
  unit->active = active;
  return unit;
}

// ------------------------------------------------------------- //

void unit_attr_add(unit_t *unit, attr_t *attr)
{
  list_add(unit->attr_list, (void*)attr);
}

// ------------------------------------------------------------- //

void *unit_attr_data_get(unit_t *unit, Uint32 type)
{
  attr_t *attr;
  list_node_t *iter = list_iter_init(unit->attr_list);
  while(attr = list_iter_next(&iter))
    if(attr->type == type)
      return attr->data;
  return NULL;
}
