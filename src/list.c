#include <malloc.h>
#include <SDL3/SDL_log.h>

#include "list.h"

// ======================== LOCAL DATA ========================= //

// ======================== LOCAL FUNC ========================= //

// ======================== GLOBAL FUNC ======================== //

list_t *list_new()
{
  list_t *list = (list_t*)malloc(sizeof(list_t));
  memset(list, 0, sizeof(list));
  return list;
}

// ------------------------------------------------------------- //

void list_add(list_t *list, void *ptr)
{
  list_node_t *new_node = (list_node_t*)malloc(sizeof(list_node_t));
  new_node->ptr = ptr;
  new_node->next = list->head;
  list->head = new_node;
}

// ------------------------------------------------------------- //

void list_del(list_t *list, void *ptr)
{
  if(list->head)
  {
    if(list->head->ptr == ptr)
    {
      list_node_t *new_head = list->head->next;
      free(list->head);
      list->head = new_head;
      return;
    }
    else
    {
      list_node_t *node = list->head;
      while(node->next) 
      {
        if(node->next->ptr == ptr)
        {
          list_node_t *node_to_del = node->next;
          node->next = node->next->next;
          free(node_to_del);
          return;
        }
      }
    }
  }
  SDL_Log("list: invalid node remove attempt [%x]", ptr);
}

// ------------------------------------------------------------- //

list_node_t *list_iter_init(list_t *list)
{
  return list->head;
}

// ------------------------------------------------------------- //

void *list_iter_next(list_node_t **iter)
{ 
  void *ptr = NULL;
  if(*iter)
  {
    ptr = (*iter)->ptr;
    *iter = (*iter)->next;
  }
  return ptr;
}

