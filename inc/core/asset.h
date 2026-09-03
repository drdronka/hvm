#ifndef __ASSET_H__
#define __ASSET_H__

#include <SDL3/SDL_render.h>

#include "gcfg.h"
#include "util.h"
#include "list.h"
#include <vector>

class asset_tex 
{
public:
  char* name;
  SDL_Texture* texture;

  asset_tex(const char *name, const char *path, SDL_Renderer *renderer);
  ~asset_tex();

  ret_e verify();
};

SDL_Texture *asset_tex_get(std::vector<asset_tex*> textures, const char* name);

#endif // __ASSET_H__