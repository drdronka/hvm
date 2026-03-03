#ifndef __ATTR_H__
#define __ATTR_H__

typedef enum 
{
  ATTR_NONE = 0,
  ATTR_PSYH,
  ATTR_VISU, 
  ATTR_DRIFT,
} attr_type_e;

typedef void (attr_f)(void *ref);

typedef struct attr 
{
  attr_type_e type;
  void *data;
  attr_f *func;
  struct attr *next;
} attr_t;


attr_t *attr_add(attr_t **head);
void attr_del(attr_t **head, attr_t *attr);

#endif // __ATTR_H__