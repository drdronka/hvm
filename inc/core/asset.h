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

ret_e asset_texture_load_all(asset_texture_t *textures, Uint32 textures_size, SDL_Renderer *renderer);
void asset_texture_free_all(asset_texture_t *textures, Uint32 textures_size);
SDL_Texture *asset_texture_get(char* name);

#endif // __ASSET_H__