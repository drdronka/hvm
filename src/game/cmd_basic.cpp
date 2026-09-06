#include <malloc.h>
#include <math.h>

#include "game_ctx.h"
#include "log.h"
#include "util.h"
#include "core_def.h"
#include "mod_basic.h"
#include "cmd_basic.h"
#include "unit.h"

// ------------------------------------------------------------- //

cmd_move_c::cmd_move_c(float dst_x, float dst_y, move_type_e type, bool temporary)
  : cmd_c(CMD_ID_MOVE, false), dst_x(dst_x), dst_y(dst_y), type(type), temporary(temporary)
{
}

cmd_move_c::~cmd_move_c()
{
}

void cmd_move_c::run()
{
  game_ctx_t *ctx = game_ctx_get();
  mod_psyh_c *psyh = (mod_psyh_c*)unit->mod_get(MOD_ID_PSYH);
  mod_visu_c *visu = (mod_visu_c*)unit->mod_get(MOD_ID_VISU);

  if(!psyh)
  {
    LOG_ERROR("no psyh mod");
    return;
  }
  
  if(!initialized)
  {
    if(type == MOVE_TYPE_REL)
      psyh->pos_rel_to_abs(&dst_x, &dst_y);

    if(visu)
      visu->anim_stage_set(ANIM_STAGE_ID_MOVE, true, false);

    initialized = true;
  }

  if(psyh->move(dst_x, dst_y, MOVE_TYPE_ABS, temporary) != RET_PENDING)
  {
    visu->anim_stage_set(ANIM_STAGE_ID_IDLE, true, true);
    unit->cmd_remove(this);
  }
}

// ------------------------------------------------------------- //

cmd_death_c::cmd_death_c() : cmd_c(CMD_ID_DEATH, true), ticks(0)
{
}

cmd_death_c::~cmd_death_c()
{
}

void cmd_death_c::run()
{
  game_ctx_t *ctx = game_ctx_get();
  mod_psyh_c *psyh = (mod_psyh_c*)unit->mod_get(MOD_ID_PSYH);
  mod_visu_c *visu = (mod_visu_c*)unit->mod_get(MOD_ID_VISU);
  
  if(ticks == 0)
  {
    visu->anim_stage_set(ANIM_STAGE_ID_DEATH, false, true);
    ticks_limit = visu->anim_ticks_get(ANIM_STAGE_ID_DEATH);
  }
  
  ticks += ctx->ticks_delta_ms;
  if(ticks > ticks_limit)
  {
    unit->dead = 1;
    unit->cmd_remove(this);
  }   
}

// ------------------------------------------------------------- //

#if 0
void attr_enter_run(void *unit_ref, void *attr_ref)
{
  unit_t *unit = (unit_t *)unit_ref;
  attr_t *attr = (attr_t *)attr_ref;

  game_ctx_t *ctx = game_ctx_get();
  attr_death_data_t *enter_data = (attr_death_data_t *)attr->data;
  attr_visu_data_t *visu_data = (attr_visu_data_t *)unit_attr_data_get(unit, ATTR_ID_VISU);
  
  if(enter_data->ticks_ms == 0)
  {
    attr_visu_anim_stage_set(visu_data, ANIM_STAGE_ID_ENTER, 0, 1);
    enter_data->ticks_limit_ms = visu_data->anim->ticks_get(ANIM_STAGE_ID_ENTER);
  }
  
  enter_data->ticks_ms += ctx->ticks_delta_ms;
  if(enter_data->ticks_ms > enter_data->ticks_limit_ms)
    attr->lcs = ATTR_LCS_CLEAN;
}

void attr_enter_clean(void *unit_ref, void *attr_ref)
{
  unit_t *unit = (unit_t *)unit_ref;

  attr_visu_anim_stage_set((attr_visu_data_t *)unit_attr_data_get(unit, ATTR_ID_VISU), ANIM_STAGE_ID_IDLE, 1, 1);
}

attr_t *attr_enter_new()
{
  attr_death_data_t *data = (attr_death_data_t *)malloc(sizeof(attr_move_data_t));
  data->ticks_ms = 0;
  return attr_new(ATTR_ID_MOVE, ATTR_TYPE_CMD, ATTR_LCS_RUN, 1, data, attr_enter_run, attr_enter_clean);
}
#endif