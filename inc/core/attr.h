#ifndef __ATTR_H__
#define __ATTR_H__

#include <SDL3/SDL_stdinc.h>

typedef void (attr_f)(void *unit_ref, void *attr_ref);

typedef struct attr 
{
  Uint32 id;
  Uint8 type;
  Uint8 lcs;
  void *data;
  attr_f *run;
  attr_f *clean;
} attr_t;

attr_t *attr_new(Uint32 id, Uint8 type, Uint8 lcs, void *data, attr_f *run, attr_f *clean);

#endif // __ATTR_H__