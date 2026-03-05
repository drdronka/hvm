#include <malloc.h>
#include <math.h>

#include "unit_impl.h"
#include "attr_impl.h"
#include "game_ctx.h"
#include "asset.h"

// ======================== LOCAL DATA ========================= //

// ======================== LOCAL FUNC ========================= //

// ======================== GLOBAL FUNC ======================== //

unit_t *unit_drifter_new()
{
  unit_t *unit = unit_new(1);
  game_ctx_t *ctx = game_ctx_get();
  
  Sint32 size = (SDL_rand(12) + 4) * 8;
  float dir = SDL_rand(10000);
  float speed = SDL_rand(3) + 1;

  unit_attr_add(
    unit, 
    attr_psyh_new(
      SDL_rand(ctx->win_x), SDL_rand(ctx->win_y),
      size, size, speed));
  unit_attr_add(unit, attr_visu_new(asset_texture_get("cage"), 1));
  unit_attr_add(unit, attr_drift_new());
  return unit;
}