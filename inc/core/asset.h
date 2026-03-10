#ifndef __ASSET_H__
#define __ASSET_H__

#include <SDL3/SDL_render.h>

#include "gcfg.h"
#include "util.h"
#include "list.h"

typedef struct asset_tex
{
  char* name;
  SDL_Texture* texture;
} asset_tex_t;

asset_tex_t *asset_tex_new(const char *name, const char *path, SDL_Renderer *renderer);
void asset_tex_del(asset_tex_t *tex);
ret_e asset_tex_verify(asset_tex_t *tex);
void asset_tex_list_destroy(list_t *list);
SDL_Texture *asset_tex_get(list_t *tex_list, char* name);
ret_e asset_tex_list_verify(list_t *list);

#endif // __ASSET_H__