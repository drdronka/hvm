#include <malloc.h>
#include <math.h>

#include "game_ctx.h"
#include "log.h"
#include "util.h"
#include "attr.h"
#include "attr_def.h"
#include "attr_cmd.h"
#include "attr_basic.h"
#include "unit.h"

// ============================================================= //

void attr_move_run(void *unit_ref, void *attr_ref)
{
  unit_t *unit = unit_ref;
  attr_t *attr = attr_ref;

  game_ctx_t *ctx = game_ctx_get();
  attr_move_data_t *move_data = attr->data;
  attr_psyh_data_t *psyh_data = unit_attr_data_get(unit, ATTR_ID_PSYH);

  if(!move_data || !psyh_data)
  {
    LOG_DEBUG("attr_move: NULL attr data: move_data[%x] psyh_data[%x]\n", move_data, psyh_data);
    attr->lcs = ATTR_LCS_CLEAN;
    return;
  }
  
  if(!move_data->initialized)
  {
    if(move_data->type == MOVE_TYPE_REL)
      attr_psyh_pos_rel_to_abs(psyh_data, &move_data->dst_x, &move_data->dst_y);

    attr_visu_anim_stage_set(unit_attr_data_get(unit, ATTR_ID_VISU), ANIM_STAGE_ID_MOVE, 1, 0);

    move_data->initialized = 1;
  }

  if(attr_psyh_move(unit, move_data->dst_x, move_data->dst_y, MOVE_TYPE_ABS, move_data->temporary) != RET_PENDING)
  {
    attr->lcs = ATTR_LCS_CLEAN;
  }
}

// ------------------------------------------------------------- //

void attr_move_clean(void *unit_ref, void *attr_ref)
{
  unit_t *unit = unit_ref;

  attr_visu_anim_stage_set(unit_attr_data_get(unit, ATTR_ID_VISU), ANIM_STAGE_ID_IDLE, 1, 1);
}

// ------------------------------------------------------------- //

attr_t *attr_move_new(float dst_x, float dst_y, move_type_e type, Uint8 temporary)
{
  attr_move_data_t *data = malloc(sizeof(attr_move_data_t));
  data->dst_x = dst_x;
  data->dst_y = dst_y;
  data->type = type;
  data->temporary = temporary;
  data->initialized = 0;
  return attr_new(ATTR_ID_MOVE, ATTR_TYPE_CMD, ATTR_LCS_RUN, 0, data, attr_move_run, attr_move_clean);
}

// ============================================================= //

void attr_death_run(void *unit_ref, void *attr_ref)
{
  unit_t *unit = unit_ref;
  attr_t *attr = attr_ref;

  game_ctx_t *ctx = game_ctx_get();
  attr_death_data_t *death_data = attr->data;
  attr_visu_data_t *visu_data = unit_attr_data_get(unit, ATTR_ID_VISU);  
  
  if(death_data->ticks_ms == 0)
  {
    attr_visu_anim_stage_set(visu_data, ANIM_STAGE_ID_DEATH, 0, 1);
    death_data->ticks_limit_ms = attr_visu_anim_stage_ticks_get(visu_data, ANIM_STAGE_ID_DEATH);
  }
  
  death_data->ticks_ms += ctx->ticks_delta_ms;
  if(death_data->ticks_ms > death_data->ticks_limit_ms)
  {
    unit->dead = 1;
    attr->lcs = ATTR_LCS_CLEAN;
  }
}

// ------------------------------------------------------------- //

attr_t *attr_death_new()
{
  attr_death_data_t *data = malloc(sizeof(attr_move_data_t));
  data->ticks_ms = 0;
  return attr_new(ATTR_ID_MOVE, ATTR_TYPE_CMD, ATTR_LCS_RUN, 1, data, attr_death_run, NULL);
}

// ============================================================= //

void attr_enter_run(void *unit_ref, void *attr_ref)
{
  unit_t *unit = unit_ref;
  attr_t *attr = attr_ref;

  game_ctx_t *ctx = game_ctx_get();
  attr_death_data_t *enter_data = attr->data;
  attr_visu_data_t *visu_data = unit_attr_data_get(unit, ATTR_ID_VISU);  
  
  if(enter_data->ticks_ms == 0)
  {
    attr_visu_anim_stage_set(visu_data, ANIM_STAGE_ID_ENTER, 0, 1);
    enter_data->ticks_limit_ms = attr_visu_anim_stage_ticks_get(visu_data, ANIM_STAGE_ID_ENTER);
  }
  
  enter_data->ticks_ms += ctx->ticks_delta_ms;
  if(enter_data->ticks_ms > enter_data->ticks_limit_ms)
    attr->lcs = ATTR_LCS_CLEAN;
}

// ------------------------------------------------------------- //

void attr_enter_clean(void *unit_ref, void *attr_ref)
{
  unit_t *unit = unit_ref;

  attr_visu_anim_stage_set(unit_attr_data_get(unit, ATTR_ID_VISU), ANIM_STAGE_ID_IDLE, 1, 1);
}

// ------------------------------------------------------------- //

attr_t *attr_enter_new()
{
  attr_death_data_t *data = malloc(sizeof(attr_move_data_t));
  data->ticks_ms = 0;
  return attr_new(ATTR_ID_MOVE, ATTR_TYPE_CMD, ATTR_LCS_RUN, 1, data, attr_enter_run, attr_enter_clean);
}

// ============================================================= //