#ifndef __ATTR_H__
#define __ATTR_H__

typedef void (attr_f)(void *ref);

typedef struct attr 
{
  Uint32 type;
  void *data;
  attr_f *run;
} attr_t;

attr_t *attr_new(Uint32 type, void *data, attr_f *run);

#endif // __ATTR_H__