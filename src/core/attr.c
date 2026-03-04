#include <malloc.h>
#include <SDL3/SDL_render.h>

#include "attr.h"

// ======================== LOCAL DATA ========================= //

// ======================== LOCAL FUNC ========================= //

// ======================== GLOBAL FUNC ======================== //

attr_t *attr_new(Uint32 type, void *data, attr_f *run)
{
  attr_t *attr = malloc(sizeof(attr_t));
  attr->type = type;
  attr->data = data;
  attr->run = run;
  return attr;
}
