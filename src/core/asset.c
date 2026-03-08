#include <SDL3/SDL_render.h>

#include "gcfg.h"
#include "log.h"
#include "asset.h"

extern asset_texture_t textures[];
extern Uint32 textures_size;

// ======================== LOCAL DATA ========================= //

// ======================== LOCAL FUNC ========================= //

// ======================== GLOBAL FUNC ======================== //

ret_t asset_texture_load_all(asset_texture_t *textures, Uint32 textures_size, SDL_Renderer *renderer)
{
  for(Uint32 n = 0; n < textures_size; n++)
  {
    LOG_INFO("asset: loading [%s]\n", textures[n].path);
    
    SDL_Surface *surface = SDL_LoadPNG(textures[n].path);
    if(!surface)
    {
      LOG_ERROR("asset: error: %s\n", SDL_GetError());
      return RET_ERR;
    }
    textures[n].texture = SDL_CreateTextureFromSurface(renderer, surface);
    if(!textures[n].texture)
    {
      LOG_ERROR("asset: error: %s\n", SDL_GetError());
      return RET_ERR;
    }
    SDL_DestroySurface(surface);
    SDL_SetTextureScaleMode(textures[n].texture, SDL_SCALEMODE_NEAREST);
  }
  return RET_OK;
}

// ------------------------------------------------------------- //

void asset_texture_free_all(asset_texture_t *textures, Uint32 textures_size)
{
  for(Uint32 n = 0; n < textures_size; n++)
  {
    SDL_DestroyTexture(textures[n].texture);
  }
}

// ------------------------------------------------------------- //

SDL_Texture *asset_texture_get(char* name)
{
  for(Uint32 n = 0; n < textures_size; n++)
  {
    if(!strncmp(name, textures[n].name, strlen(name)))
      return textures[n].texture;
  }
  return NULL;
}

// ------------------------------------------------------------- //