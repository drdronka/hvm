#include <malloc.h>
#include <math.h>

#include "unit_impl.h"
#include "unit_def.h"
#include "attr_basic.h"
#include "game_ctx.h"
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

  anim_t *anim = anim_new();
  anim_stage_t *stage_idle = anim_stage_new(ANIM_STAGE_ID_IDLE);
  anim_stage_add_step(stage_idle, anim_step_new(asset_texture_get("worm_idle_0"), 600));
  anim_stage_add_step(stage_idle, anim_step_new(asset_texture_get("worm_idle_1"), 600));
  anim_stage_t *stage_move = anim_stage_new(ANIM_STAGE_ID_MOVE);
  anim_stage_add_step(stage_move, anim_step_new(asset_texture_get("worm_move_0"), 150));
  anim_stage_add_step(stage_move, anim_step_new(asset_texture_get("worm_move_1"), 150));
  anim_add_stage(anim, stage_idle);
  anim_add_stage(anim, stage_move);
  unit_attr_add(unit, attr_visu_new(anim, ANIM_STAGE_ID_IDLE));

  unit_attr_add(unit, attr_wander_new(size / 2, 4000));

  return unit;
}

// ------------------------------------------------------------- //