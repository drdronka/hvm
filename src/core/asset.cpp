#include <malloc.h>
#include <string.h>
#include <SDL3/SDL_render.h>

#include "log.h"
#include "list.h"
#include "asset.h"
#include <vector>

// ------------------------------------------------------------- //

asset_tex::asset_tex(const char *name, const char *path, SDL_Renderer *renderer)
{
  LOG_DEBUG("name[%s] path[%s]\n", name, path);

  this->name = (char *)malloc(strlen(name) + 1);
  memcpy(this->name, name, strlen(name) + 1);
  
  SDL_Surface *surf = SDL_LoadPNG(path);
  if(!surf)
  {
    LOG_ERROR("SDL: %s\n", SDL_GetError());
    return;
  }

  texture = SDL_CreateTextureFromSurface(renderer, surf);

  if(!texture)
  {
    LOG_ERROR("SDL: %s\n", SDL_GetError());
    return;
  }

  SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);
  SDL_DestroySurface(surf);
}

asset_tex::~asset_tex()
{
  LOG_DEBUG("name[%x]\n", name);

  if(texture) 
    SDL_DestroyTexture(texture);

  if(name) 
    free(name);
}

ret_e asset_tex::verify()
{
  if(!texture)
  {
    LOG_ERROR("NULL texture");
    return RET_ERR;
  }

  return RET_OK;
}

SDL_Texture *asset_tex_get(std::vector<asset_tex*> textures, const char* name)
{
  for(const auto& tex : textures)
    if(!strcmp(name, tex->name))
      return tex->texture;

  LOG_ERROR("tex not found: name[%s]\n", name);

  return NULL;
}

// ------------------------------------------------------------- //