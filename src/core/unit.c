#include <malloc.h>

#include "log.h"
#include "gcfg.h"
#include "unit.h"
#include "unit_def.h"
#include "attr.h"
#include "attr_def.h"
#include "game.h"
#include "asset.h"

// ======================== LOCAL DATA ========================= //

// ======================== LOCAL FUNC ========================= //

// ======================== GLOBAL FUNC ======================== //

unit_t *unit_new(unit_id_e unit_id)
{
  unit_t *unit = malloc(sizeof(unit_t));
  memset(unit, 0, sizeof(unit_t));
  unit->attr_list = list_new();
  unit->id = unit_id;
  return unit;
}

// ------------------------------------------------------------- //

void unit_attr_add(unit_t *unit, attr_t *attr)
{
  list_add(unit->attr_list, (void*)attr);
}

// ------------------------------------------------------------- //

void unit_attr_add_head(unit_t *unit, attr_t *attr)
{
  list_add_head(unit->attr_list, attr);
}

// ------------------------------------------------------------- //

void unit_attr_del(unit_t *unit, attr_t *attr)
{
  if(attr->clean) attr->clean(unit, attr);
  if(attr->data) free(attr->data);
  list_del(unit->attr_list, attr);
}

// ------------------------------------------------------------- //

void *unit_attr_data_get(unit_t *unit, unit_id_e id)
{
  attr_t *attr;
  list_node_t *iter = list_iter_init(unit->attr_list);
  while(attr = list_iter_next(&iter))
    if(attr->id == id)
      return attr->data;
  return NULL;
}

// ------------------------------------------------------------- //

void *unit_cmd_clear_all(unit_t *unit)
{
  attr_t *attr;
  list_node_t *iter = list_iter_init(unit->attr_list);
  while(attr = list_iter_next(&iter))
    if(attr->type == ATTR_TYPE_CMD)
      attr->lcs = ATTR_LCS_CLEAN;
  return NULL;
}

// ------------------------------------------------------------- //

Uint8 unit_cmd_is_empty(unit_t *unit)
{
  attr_t *attr;
  list_node_t *iter = list_iter_init(unit->attr_list);
  while(attr = list_iter_next(&iter))
    if(attr->type == ATTR_TYPE_CMD)
      return 0;
  return 1;
}