#ifndef __ATTR_H__
#define __ATTR_H__

#include <SDL3/SDL_stdinc.h>

#include "attr_def.h"

typedef void (attr_f)(void *unit_ref, void *attr_ref);

typedef struct attr 
{
  attr_id_e id;
  attr_type_e type;
  attr_lcs_e lcs;
  void *data;
  attr_f *run;
  attr_f *clean;
} attr_t;

attr_t *attr_new(attr_id_e id, attr_type_e type, attr_lcs_e lcs, void *data, attr_f *run, attr_f *clean);

#endif // __ATTR_H__