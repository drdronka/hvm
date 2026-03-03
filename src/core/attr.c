#include <malloc.h>
#include <SDL3/SDL_stdinc.h>

#include "attr.h"

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
  Uint8 found = 0;

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