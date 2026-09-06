#include <malloc.h>
#include <math.h>

#include "util.h"
#include "game_ctx.h"
#include "mod_basic.h"
#include "unit_basic.h"
#include "core_def.h"
#include "asset.h"
#include "anim.h"

// ------------------------------------------------------------- //

unit_c *unit_worm_new(float pos_x, float pos_y)
{
  unit_c *unit = new unit_c(UNIT_ID_WORM);
  game_ctx_t *ctx = game_ctx_get();
  
  Sint32 size = 96;
  float dir = FRAND(2 * M_PI);
  float speed = 3;

  unit->mod_add(new mod_psyh_c(pos_x, pos_y, size, size, speed, dir));
  unit->mod_add(new mod_visu_c(anim_get(ctx->anims, "worm"), ANIM_STAGE_ID_IDLE));
  //unit->mod_add(attr_wander_new(size / 2, 4000));

  return unit;
}
