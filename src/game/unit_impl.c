#include <malloc.h>
#include <math.h>

#include "unit_impl.h"
#include "attr_impl.h"
#include "game_context.h"
#include "asset.h"

// ======================== LOCAL DATA ========================= //

// ======================== LOCAL FUNC ========================= //

// ======================== GLOBAL FUNC ======================== //

unit_t *unit_drifter_new()
{
  unit_t *unit = unit_new(1);
  game_context_t *context = game_context_get();
  
  Sint32 size = (SDL_rand(12) + 4) * 16;
  float dir_seed = SDL_rand(10000);
  float speed = SDL_rand(5) + 1;
  speed *= speed;

  unit_attr_add(
    unit, 
    attr_psyh_new(
      SDL_rand(context->win_x), 
      SDL_rand(context->win_y), 
      sin(dir_seed) * speed,
      cos(dir_seed) * speed,
      size,
      size));
  unit_attr_add(unit, attr_visu_new(asset_texture_get("cage"), 1));
  unit_attr_add(unit, attr_drift_new());
  return unit;
}