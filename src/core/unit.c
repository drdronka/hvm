#include <malloc.h>

#include "log.h"
#include "unit.h"
#include "unit_def.h"
#include "attr.h"
#include "attr_def.h"

// ======================== LOCAL DATA ========================= //

// ======================== LOCAL FUNC ========================= //

// ======================== GLOBAL FUNC ======================== //

unit_t *unit_new(unit_id_e unit_id)
{
  LOG_DEBUG("unit_new: id[%u]\n", unit_id);

  unit_t *unit = malloc(sizeof(unit_t));
  memset(unit, 0, sizeof(unit_t));
  unit->attr_list = list_new();
  unit->id = unit_id;

  return unit;
}

// ------------------------------------------------------------- //

void unit_del(unit_t *unit)
{
  if(!unit)
  {
    LOG_ERROR("unit_del: NULL unit\n");
    return;
  }

  LOG_DEBUG("unit_del: id[%u]\n", unit->id);
  attr_list_destroy(unit->attr_list);
  free(unit);
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

void unit_list_destroy(list_t *list)
{
  if(!list)
  {
    LOG_ERROR("unit_list_destroy: NULL list\n");
    return;
  }
  LOG_DEBUG("unit_list_destroy: list[0x%x]\n", list);

  unit_t *unit;
  list_node_t *unit_iter = list_iter_init(list);
  while(unit = list_iter_next(&unit_iter))
  {
    unit_del(unit);
    list_del(list, unit);
  }
  list_destroy(list);
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

// ------------------------------------------------------------- //

void unit_attr_run(unit_t *unit, attr_id_e id, attr_type_e type)
{
  attr_t *attr;
  list_node_t *iter = list_iter_init(unit->attr_list);
  while(attr = list_iter_next(&iter))
    if(attr->lcs == ATTR_LCS_RUN)
      if(id == ATTR_ID_ANY || attr->id == id)
        if(type == ATTR_TYPE_ANY || attr->type == type)
        {
          attr->run(unit, attr);
          if(type == ATTR_TYPE_CMD)
            break;
        }
}

// ------------------------------------------------------------- //

void unit_attr_clean(unit_t *unit, attr_id_e id, attr_type_e type)
{
  attr_t *attr;
  list_node_t *iter = list_iter_init(unit->attr_list);
  while(attr = list_iter_next(&iter))
    if(attr->lcs == ATTR_LCS_CLEAN)
      if(id == ATTR_ID_ANY || attr->id == id)
        if(type == ATTR_TYPE_ANY || attr->type == type)
        {
          if(attr->clean) 
            attr->clean(unit, attr);
          attr_del(attr);
          list_del(unit->attr_list, attr);
        }
}

// ------------------------------------------------------------- //

void unit_list_attr_run(list_t *list, attr_id_e id, attr_type_e type)
{
  unit_t *unit;
  list_node_t *iter = list_iter_init(list);
  while(unit = list_iter_next(&iter))
    unit_attr_run(unit, id, type);
}

// ------------------------------------------------------------- //

void unit_list_attr_clean(list_t *list, attr_id_e id, attr_type_e type)
{
  unit_t *unit;
  list_node_t *iter = list_iter_init(list);
  while(unit = list_iter_next(&iter))
    unit_attr_clean(unit, id, type);
}
