#ifndef __VISU_H__
#define __VISU_H__

#include <SDL3/SDL_render.h>

#include "attr.h"

typedef struct visu
{
  uint8_t visible;
  SDL_Renderer *renderer;
  SDL_Texture *tex;
} visu_t;

visu_t *visu_new();
void behv_draw(void *ref);

#endif // __VISU_H__