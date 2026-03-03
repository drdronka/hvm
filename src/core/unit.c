#include <malloc.h>
#include <SDL3/SDL_log.h>

#include "gcfg.h"
#include "unit.h"
#include "attr.h"

extern Uint32 g_delta;

unit_t *unit_add(unit_t **head)
{
  unit_t *unit = (unit_t*)malloc(sizeof(unit_t));
  memset(unit, 0, sizeof(unit_t));
  unit->next = *head;
  *head = unit;
  return unit;
}

void unit_del(unit_t **head, unit_t *unit)
{
  uint8_t found = 0;

  if(*head == NULL)
    return;

  if(unit == *head)
  {
    if(unit->next)
      *head = unit->next;
    found = 1;
  }

  unit_t *attr_tmp = *head;
  while(attr_tmp)
  {
    if(attr_tmp->next == unit)
    {
      attr_tmp = unit->next;
      found = 1;
    }
  }

  if(found)
  {
    while((volatile attr_t*)unit->attr_head)
      attr_del(&unit->attr_head, unit->attr_head);
    free(unit);
  }
}

void unit_proc(unit_t *unit)
{
  attr_t *attr = unit->attr_head;
  while(attr)
  {
    //SDL_Log("proc type [%d]", attr->type);
    //if(attr->type == ATTR_BEHV)
    //  ((behv_t*)(attr->data))->f((void*)unit);
    attr = attr->next;
  }
}

attr_t *unit_attr_get(unit_t *unit, attr_type_e type)
{
  attr_t *attr = unit->attr_head;
  while(attr)
  {
    //SDL_Log("attr_get [%d] looking for [%d]", attr->type, type);
    if(attr->type == type)
      return attr->data;
    attr = attr->next;
  }
  return NULL;
}

attr_t *unit_attr_add(unit_t *unit)
{
  return attr_add(&unit->attr_head);
}
