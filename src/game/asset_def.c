#include "SDL3/SDL_stdinc.h"
#include "asset.h"
#include "util.h"

asset_texture_t textures[] = 
{
  {"black_square", "assets/img/black_square.png", NULL},
  {"cage", "assets/img/cage.png", NULL},
  {"tank", "assets/img/tank.png", NULL},
  {"worm_0", "assets/img/worm_0.png", NULL},
  {"worm_1", "assets/img/worm_1.png", NULL},
};
const Uint32 textures_size = sizeof(textures) / sizeof(asset_texture_t);