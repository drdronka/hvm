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
  attr_f *run;
} attr_t;


attr_t *attr_new(attr_type_e type, void *data, attr_f *run);

#endif // __ATTR_H__