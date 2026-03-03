#include <SDL3/SDL_render.h>
#include <SDL3/SDL_log.h>

#include "asset.h"
#include "gcfg.h"
#include "game.h"

typedef struct asset_texture 
{
  const char* name;
  const char* path;
  SDL_Texture* texture;
} asset_texture_t;

asset_texture_t textures[] = 
{
  {"black_square", "assets/img/black_square.png", NULL},
  {"cage", "assets/img/cage.png", NULL},
};

ret_e asset_texture_load_all()
{
  for(Uint32 n = 0; n < sizeof(textures) / sizeof(asset_texture_t); n++)
  {
    SDL_Surface *surface = SDL_LoadPNG(textures[n].path);
    if(!surface)
    {
      SDL_Log("asset: error: %s", SDL_GetError());
      return RET_ERR;
    }
    textures[n].texture = SDL_CreateTextureFromSurface(game_context_get()->renderer, surface);
    if(!textures[n].texture)
    {
      SDL_Log("asset: errpr: %s", SDL_GetError());
      return RET_ERR;
    }
    SDL_DestroySurface(surface);
  }
  return RET_OK;
}

void asset_texture_free_all()
{
  for(Uint32 n = 0; n < sizeof(textures) / sizeof(asset_texture_t); n++)
  {
    SDL_DestroyTexture(textures[n].texture);
  }
}

SDL_Texture *asset_texture_get(char* name)
{
  for(Uint32 n = 0; n < sizeof(textures) / sizeof(asset_texture_t); n++)
  {
    if(!strncmp(name, textures[n].name, strlen(name)))
      return textures[n].texture;
  }
  return NULL;
}