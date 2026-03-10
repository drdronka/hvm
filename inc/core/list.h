#ifndef __LIST_H__
#define __LIST_H__

typedef struct list_node
{
  void *ptr;
  struct list_node *next;
} list_node_t;

typedef struct list
{
  list_node_t *head;
} list_t;

list_t *list_new();
void list_add(list_t *list, void *ptr);
void list_add_head(list_t *list, void *ptr);
void list_del(list_t *list, void *ptr);
void list_destroy(list_t *list);
list_node_t *list_iter_init(list_t *list);
void *list_iter_next(list_node_t **iter);

#endif // __LIST_H__