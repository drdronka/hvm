#include "SDL3/SDL_stdinc.h"
#include "asset.h"

asset_texture_t textures[] = 
{
  {"black_square", "assets/img/black_square.png", NULL},
  {"cage", "assets/img/cage.png", NULL},
};
const Uint32 textures_size = sizeof(textures) / sizeof(asset_texture_t);