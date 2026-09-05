#pragma once

#include <SDL3/SDL_render.h>

#include "gcfg.h"
#include "util.h"
#include <vector>

class asset_tex_c
{
public:
  char* name;
  SDL_Texture* texture;

  asset_tex_c(const char *name, const char *path, SDL_Renderer *renderer);
  ~asset_tex_c();

  ret_e verify();
};

SDL_Texture *asset_tex_get(std::vector<asset_tex_c*> textures, const char* name);
