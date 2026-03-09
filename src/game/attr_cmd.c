#include <malloc.h>
#include <math.h>

#include "attr.h"
#include "attr_def.h"
#include "attr_cmd.h"
#include "attr_basic.h"
#include "unit.h"
#include "game_ctx.h"
#include "log.h"
#include "util.h"

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
    // change relative coords to absolute
    if(move_data->move_type == ATTR_PSYH_MOVE_TYPE_REL)
      attr_psyh_rel_to_abs(psyh_data, &move_data->dst_x, &move_data->dst_y);

    // change animation
    attr_visu_anim_stage_set(unit_attr_data_get(unit, ATTR_ID_VISU));

    move_data->initialized = 1;
  }

  if(attr_psyh_move(
    psyh_data, 
    move_data->dst_x, 
    move_data->dst_y, 
    ATTR_PSYH_MOVE_TYPE_ABS) 
    == ATTR_PSYH_MOVE_RET_FINISHED)
  {
    attr->lcs = ATTR_LCS_CLEAN;
  }
}

// ------------------------------------------------------------- //

void attr_move_clean(void *unit_ref, void *attr_ref)
{
  unit_t *unit = unit_ref;
  attr_t *attr = attr_ref;

  attr_move_data_t *data = attr->data;

  // change animation
  attr_visu_data_t *visu_data = unit_attr_data_get(unit, ATTR_ID_VISU);
  if(visu_data)
    visu_data->anim_stage_id = ANIM_STAGE_ID_IDLE;

  // update wander attr
  attr_wander_data_t *wander_data = unit_attr_data_get(unit, ATTR_ID_WANDER);
  if(!data->temporary && wander_data)
  {
    attr_psyh_data_t *psyh_data = unit_attr_data_get(unit, ATTR_ID_PSYH);
    if(psyh_data)
    {
      float pos_x;
      float pos_y;
      attr_psyh_pos_get(psyh_data, &pos_x, &pos_y);
      attr_wander_pos_set(wander_data, pos_x, pos_y);
    }
  }
}

// ------------------------------------------------------------- //

attr_t *attr_move_new(float dst_x, float dst_y, Uint8 move_type, Uint8 temporary)
{
  attr_move_data_t *data = malloc(sizeof(attr_move_data_t));
  data->dst_x = dst_x;
  data->dst_y = dst_y;
  data->move_type = move_type;
  data->initialized = 0;
  data->temporary = temporary;
  return attr_new(ATTR_ID_MOVE, ATTR_TYPE_CMD, ATTR_LCS_RUN, data, attr_move_run, attr_move_clean);
}

// ============================================================= //