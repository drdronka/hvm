#ifndef __ASSET_H__
#define __ASSET_H__

#include <SDL3/SDL_render.h>

#include "gcfg.h"

typedef struct asset_texture 
{
  const char* name;
  const char* path;
  SDL_Texture* texture;
} asset_texture_t;

ret_e asset_texture_load_all();
void asset_texture_free_all();
SDL_Texture *asset_texture_get(char* name);

#endif // __ASSET_H__