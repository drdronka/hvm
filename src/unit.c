#include <malloc.h>
#include <SDL3/SDL_log.h>

#include "unit.h"
#include "gcfg.h"

extern Uint32 g_delta;

/// ATTR ///

attr_t *attr_add(attr_t **head)
{
  attr_t *attr = (attr_t*)malloc(sizeof(attr_t));
  memset(attr, 0, sizeof(attr_t));
  attr->next = *head;
  *head = attr;
  return attr;
}

void attr_del(attr_t **head, attr_t *attr)
{
  uint8_t found = 0;

  if(*head == NULL)
    return;

  if(attr == *head)
  {
    if(attr->next)
      *head = attr->next;
    found = 1;
  }

  attr_t *attr_tmp = *head;
  while(attr_tmp)
  {
    if(attr_tmp->next == attr)
    {
      attr_tmp = attr->next;
      found = 1;
    }
  }

  if(found)
  {
    if(attr->data) 
      free(attr->data);
    free(attr);
  }
}

/// UNIT ///

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
    if(attr->type == ATTR_BEHV)
      ((behv_t*)(attr->data))->f((void*)unit);
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

/// BEHV ///

behv_t *behv_new()
{
  behv_t *behv = (behv_t*)malloc(sizeof(behv_t));
  memset(behv, 0, sizeof(behv_t));
  return behv;
}

void behv_drift(void *ref)
{
  //SDL_Log("drift");
  unit_t *unit = (unit_t*)ref;
  if(unit->active)
  {
    psyh_t *psyh = (psyh_t*)unit_attr_get(unit, ATTR_PSYH);
    //SDL_Log("psyh[%x]", psyh);
    if(psyh)
    {
      float mov_x = psyh->vel_x * (g_delta / 100000);
      float mov_y = psyh->vel_y * (g_delta / 100000);

      if(psyh->pos_x + mov_x < 0 || psyh->pos_x + mov_x > WINX)
      {
        psyh->vel_x = 0 - psyh->vel_x;
        mov_x = psyh->vel_x * (g_delta / 100000);
      }
      if(psyh->pos_y + mov_y < 0 || psyh->pos_y + mov_y > WINY)
      {
        psyh->vel_y = 0 - psyh->vel_y;
        mov_y = psyh->vel_y * (g_delta / 100000);
      }

      psyh->pos_x += mov_x;
      psyh->pos_y += mov_y;
    }
  }
}

void behv_draw(void *ref)
{
  //SDL_Log("draw");
  unit_t *unit = (unit_t*)ref;
  if(unit->active)
  {
    psyh_t *psyh = (psyh_t*)unit_attr_get(unit, ATTR_PSYH);
    visu_t *visu = (visu_t*)unit_attr_get(unit, ATTR_VISU);
    //SDL_Log("psyh[%x] visu[%x]", psyh, visu);
    if(psyh && visu)
    {
      SDL_FRect rect;
      rect.x = psyh->pos_x - (psyh->size_x / 2);
      rect.y = psyh->pos_y - (psyh->size_y / 2);
      rect.w = psyh->size_x;
      rect.h = psyh->size_y;
      //SDL_Log("drawing [%f][%f][%f][%f]", rect.x, rect.y, rect.w, rect.h);
      SDL_RenderTexture(visu->renderer, visu->tex, NULL, &rect);
    }
  }
}

/// PSYH ///

psyh_t *psyh_new()
{
  psyh_t *psyh = (psyh_t*)malloc(sizeof(psyh_t));
  memset(psyh, 0, sizeof(psyh_t));
  return psyh;
}

/// VISU ///

visu_t *visu_new()
{
  visu_t *visu = (visu_t*)malloc(sizeof(visu_t));
  memset(visu, 0, sizeof(visu_t));
  return visu;
}