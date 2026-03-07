#include <malloc.h>
#include <math.h>

#include "attr.h"
#include "attr_def.h"
#include "attr_cmd.h"
#include "attr_perk.h"
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
  }
  
  if(move_data && psyh_data)
  {
    // set direction
    psyh_data->dir = SDL_atan((move_data->dst_y - psyh_data->pos_y) / (move_data->dst_x - psyh_data->pos_x));
    if(move_data->dst_x < psyh_data->pos_x) 
      psyh_data->dir += M_PI;

    // calculate destination
    float dst_x = psyh_data->pos_x + psyh_data->speed * cos(psyh_data->dir) * ctx->move_mult;
    float dst_y = psyh_data->pos_y + psyh_data->speed * sin(psyh_data->dir) * ctx->move_mult;
    float dist = psyh_data->speed * ctx->move_mult;    

    if(ABS_DIST(move_data->dst_x, move_data->dst_y, dst_x, dst_y) < dist)
    {
      // destination reached
      psyh_data->pos_x = move_data->dst_x;
      psyh_data->pos_y = move_data->dst_y;
      attr->lcs = ATTR_LCS_CLEAN;
    }
    else
    {
      // destination not reached
      psyh_data->pos_x = dst_x;
      psyh_data->pos_y = dst_y;
    }
    
    // out of screen check
    //if(data->pos_x < 0) data->pos_x = 0 - data->pos_x;
    //if(data->pos_x > FWINX) data->pos_x = FWINX - (data->pos_x - FWINX);
    //if(data->pos_y < 0) data->pos_y = 0 - data->pos_y;
    //if(data->pos_y > FWINY) data->pos_y = FWINY - (data->pos_y - FWINY);
  }
}

// ------------------------------------------------------------- //

void attr_move_clean(void *unit_ref, void *attr_ref)
{
  attr_t *attr = attr_ref;
  if(attr->data) free(attr->data);
}

// ------------------------------------------------------------- //

attr_t *attr_move_new(float dst_x, float dst_y)
{
  attr_move_data_t *data = malloc(sizeof(attr_move_data_t));
  data->dst_x = dst_x;
  data->dst_y = dst_y;
  return attr_new(ATTR_ID_MOVE, ATTR_TYPE_CMD, ATTR_LCS_RUN, data, attr_move_run, attr_move_clean);
}

// ============================================================= //