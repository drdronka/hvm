#ifndef __ATTR_H__
#define __ATTR_H__

typedef void (attr_f)(void *ref);

typedef struct attr 
{
  Uint32 id;
  Uint8 type;
  Uint8 lcs;
  void *data;
  attr_f *run;
} attr_t;

attr_t *attr_new(Uint32 id, Uint8 type, Uint8 lcs, void *data, attr_f *run, attr_f *clean);

#endif // __ATTR_H__