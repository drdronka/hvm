#include <malloc.h>
#include <SDL3/SDL_log.h>

#include "unit.h"
#include "unit_impl.h"
#include "attr.h"
#include "attr_impl.h"
#include "game.h"
#include "asset.h"

// ------------------------------------------------------------- //

unit_t *unit_drifter_new()
{
  unit_t *unit = unit_new(1);
  game_context_t *context = game_context_get();
  Sint32 size = (SDL_rand(12) + 4) * 16;
  unit_attr_add(
    unit, 
    attr_psyh_new(
      SDL_rand(context->win_x), 
      SDL_rand(context->win_y), 
      (float)SDL_rand(21) - 10,
      (float)SDL_rand(21) - 10,
      size,
      size));
  unit_attr_add(unit, attr_visu_new(asset_texture_get("cage"), 1));
  unit_attr_add(unit, attr_drift_new());
  return unit;
}