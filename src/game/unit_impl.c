#include <malloc.h>
#include <math.h>

#include "game_ctx.h"
#include "unit_impl.h"
#include "unit_def.h"
#include "attr_basic.h"
#include "asset.h"
#include "anim.h"

// ======================== LOCAL DATA ========================= //

// ======================== LOCAL FUNC ========================= //

// ======================== GLOBAL FUNC ======================== //

unit_t *unit_worm_new(float pos_x, float pos_y)
{
  unit_t *unit = unit_new(UNIT_ID_WORM);
  game_ctx_t *ctx = game_ctx_get();
  
  Sint32 size = 96;
  float dir = SDL_rand(2 * M_PI);
  float speed = 3;

  unit_attr_add(unit, attr_psyh_new(pos_x, pos_y, size, size, speed, dir));
  unit_attr_add(unit, attr_visu_new(anim_get(ctx->anim_list, "worm"), ANIM_STAGE_ID_IDLE));
  unit_attr_add(unit, attr_wander_new(size / 2, 4000));

  return unit;
}

// ------------------------------------------------------------- //