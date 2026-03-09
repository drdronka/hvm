#include <malloc.h>
#include <SDL3/SDL_render.h>

#include "attr.h"
#include "attr_def.h"

// ======================== LOCAL DATA ========================= //

// ======================== LOCAL FUNC ========================= //

// ======================== GLOBAL FUNC ======================== //

attr_t *attr_new(attr_id_e id, attr_type_e type, attr_lcs_e lcs, void *data, attr_f *run, attr_f *clean)
{
  attr_t *attr = malloc(sizeof(attr_t));
  memset(attr, 0, sizeof(attr));
  attr->id = id;
  attr->type = type;
  attr->lcs = lcs;
  attr->data = data;
  attr->run = run;
  attr->clean = clean;
  return attr;
}
