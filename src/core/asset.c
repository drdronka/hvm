#include <SDL3/SDL_render.h>
#include <SDL3/SDL_log.h>

#include "asset.h"
#include "gcfg.h"
#include "game.h"

extern asset_texture_t textures[];
extern Uint32 textures_size;

ret_e asset_texture_load_all()
{
  for(Uint32 n = 0; n < textures_size; n++)
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
  for(Uint32 n = 0; n < textures_size; n++)
  {
    SDL_DestroyTexture(textures[n].texture);
  }
}

SDL_Texture *asset_texture_get(char* name)
{
  for(Uint32 n = 0; n < textures_size; n++)
  {
    if(!strncmp(name, textures[n].name, strlen(name)))
      return textures[n].texture;
  }
  return NULL;
}