#include <malloc.h>
#include <math.h>

#include "util.h"
#include "game.h"
#include "mod_basic.h"
#include "unit_basic.h"
#include "core_def.h"
#include "anim.h"

// ------------------------------------------------------------- //

unit_c *unit_worm_new(float pos_x, float pos_y)
{
  unit_c *unit = new unit_c(UNIT_ID_WORM);
  game_c *game = game_c::get();
  
  Uint32 size = 96;
  float dir = FRAND(2 * M_PI);
  float speed = 3;

  unit->mod_add(new mod_psyh_c(pos_x, pos_y, size, size, speed, dir));
  unit->mod_add(new mod_visu_c(anim_get(game->anims, "worm"), "idle", true));
  unit->mod_add(new mod_wander_c(size / 2, 4000));

  return unit;
}
