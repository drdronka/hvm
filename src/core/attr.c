#include <malloc.h>
#include <SDL3/SDL_stdinc.h>

#include "attr.h"

attr_t *attr_new(attr_type_e type, void *data, attr_f *run)
{
  attr_t *attr = (attr_t*)malloc(sizeof(attr_t));
  attr->type = type;
  attr->data = data;
  attr->run = run;
  return attr;
}
