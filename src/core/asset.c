#include <malloc.h>
#include <SDL3/SDL_render.h>

#include "gcfg.h"
#include "log.h"
#include "list.h"
#include "asset.h"
#include "game_ctx.h"

// ======================== LOCAL DATA ========================= //

// ======================== LOCAL FUNC ========================= //

// ======================== GLOBAL FUNC ======================== //

asset_tex_t *asset_tex_new(const char *name, const char *path)
{
  LOG_DEBUG("asset_tex_new: name[%s] path[%s]\n", name, path);

  asset_tex_t *tex = malloc(sizeof(asset_tex_t));

  SDL_Surface *surf = SDL_LoadPNG(path);
  if(!surf)
  {
    LOG_ERROR("asset_tex_new: SDL: %s\n", SDL_GetError());
    free(tex);
    return NULL;
  }
  tex->texture = SDL_CreateTextureFromSurface(game_ctx_get()->renderer, surf);

  if(!tex->texture)
  {
    LOG_ERROR("asset_tex_new: SDL: %s\n", SDL_GetError());
    free(tex);
    return NULL;
  }

  SDL_SetTextureScaleMode(tex->texture, SDL_SCALEMODE_NEAREST);
  SDL_DestroySurface(surf);

  tex->name = malloc(strlen(name) + 1);
  memcpy(tex->name, name, strlen(name) + 1);

  return tex;
}

// ------------------------------------------------------------- //

void asset_tex_del(asset_tex_t *tex)
{
  if(!tex)
  {
    LOG_ERROR("asset_tex_del: NULL tex\n");
    return;
  }

  LOG_DEBUG("asset_tex_del: tex[0x%x]\n", tex);
  if(tex->texture) SDL_DestroyTexture(tex->texture);
  if(tex->name) free(tex->name);
  free(tex);
}

// ------------------------------------------------------------- //

ret_e asset_tex_verify(asset_tex_t *tex)
{
  if(!tex)
  {
    LOG_ERROR("asset_tex_verify: NULL tex");
    return RET_ERR;
  }
  if(!tex->texture)
  {
    LOG_ERROR("asset_tex_verify: NULL texture");
    return RET_ERR;
  }

  return RET_OK;
}
// ------------------------------------------------------------- //

void asset_tex_list_destroy(list_t *list)
{
  if(!list)
  {
    LOG_ERROR("asset_tex_list_destroy: NULL list");
    return;
  }
  LOG_DEBUG("asset_tex_list_destroy: list[0x%x]\n", list);

  asset_tex_t *tex;
  list_node_t *iter = list_iter_init(list);
  while(tex = list_iter_next(&iter))
  {
    asset_tex_del(tex);
    list_del(list, tex);
  }
  list_destroy(list);
}

// ------------------------------------------------------------- //

SDL_Texture *asset_tex_get(list_t *tex_list, char* name)
{
  asset_tex_t *tex;
  list_node_t *iter = list_iter_init(tex_list);
  while(tex = list_iter_next(&iter))
    if(!strncmp(name, tex->name, strlen(name)))
      return tex->texture;

  LOG_ERROR("asset_tex_del: tex not found: name[%s]\n", name);

  return NULL;
}

// ------------------------------------------------------------- //

ret_e asset_tex_list_verify(list_t *list)
{
  if(!list)
  {
    LOG_ERROR("asset_tex_list_verify: NULL list\n");
    return RET_ERR;
  }
  LOG_DEBUG("asset_tex_list_verify: list[0x%x]\n", list);

  asset_tex_t *tex;
  list_node_t *iter = list_iter_init(list);
  while(tex = list_iter_next(&iter))
    if(!asset_tex_verify(tex))
      return RET_ERR;

  return RET_OK;
}

// ------------------------------------------------------------- //