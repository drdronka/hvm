#ifndef __ATTR_H__
#define __ATTR_H__

#include <SDL3/SDL_stdinc.h>

#include "attr_def.h"
#include "list.h"

typedef void (attr_f)(void *unit_ref, void *attr_ref);

typedef struct attr 
{
  attr_id_e id;
  attr_type_e type;
  attr_lcs_e lcs;
  Uint8 protected;
  void *data;
  attr_f *run;
  attr_f *clean;
} attr_t;

attr_t *attr_new(
  attr_id_e id, 
  attr_type_e type, 
  attr_lcs_e lcs, 
  Uint8 protected, 
  void *data, 
  attr_f *run, 
  attr_f *clean);
void attr_del(attr_t *attr);
void attr_list_destroy(list_t *list);

#endif // __ATTR_H__