#ifndef __VISU_H__
#define __VISU_H__

#include <SDL3/SDL_render.h>

#include "attr.h"

typedef struct visu_data
{  
  SDL_Texture *tex;
  Uint8 visible;
} visu_data_t;

attr_t *visu_new(SDL_Texture *tex, Uint8 visible);

#endif // __VISU_H__