#include <malloc.h>
#include <string.h>

#include "log.h"
#include "list.h"

// ======================== LOCAL DATA ========================= //

// ======================== LOCAL FUNC ========================= //

// ======================== GLOBAL FUNC ======================== //

list_t *list_new()
{
  list_t *list = malloc(sizeof(list_t));
  memset(list, 0, sizeof(list));
  return list;
}

// ------------------------------------------------------------- //

void list_add(list_t *list, void *ptr)
{
  list_node_t *new_node = malloc(sizeof(list_node_t));
  new_node->ptr = ptr;
  new_node->next = NULL;

  if(!list->head)
  {
    list->head = new_node;
  }
  else
  {
    list_node_t *node_it = list->head;
    while(node_it->next)
    {
      node_it = node_it->next;
    }
    node_it->next = new_node;
  }
}

// ------------------------------------------------------------- //

void list_add_head(list_t *list, void *ptr)
{
  list_node_t *new_node = malloc(sizeof(list_node_t));
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
        node = node->next;
      }
    }
  }
  LOG_ERROR("invalid node remove [%x]", ptr);
}

// ------------------------------------------------------------- //

void list_destroy(list_t *list)
{
  if(!list) 
  {
    LOG_ERROR("NULL list");
    return;
  }
  LOG_DEBUG("list[%x]\n", list);
  free(list);
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
