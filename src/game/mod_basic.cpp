#include <malloc.h>
#include <string.h>
#include <SDL3/SDL_render.h>
#include <math.h>

#include "gcfg.h"
#include "game_ctx.h"
#include "log.h"
#include "util.h"
#include "mod_basic.h"
#include "cmd_basic.h"
#include "unit.h"
#include "anim.h"
#include "gui.h"

// ------------------------------------------------------------- //

mod_psyh_c::mod_psyh_c(
  float pos_x, float pos_y, float size_x, float size_y, float speed, float dir)
  : mod_c(MOD_ID_PSYH, MOD_TYPE_BASIC, MOD_LCS_RUN), 
  pos_x(pos_x), pos_y(pos_y), size_x(size_x), size_y(size_y), speed(speed), dir(dir)
{
}

mod_psyh_c::~mod_psyh_c()
{
}

ret_e mod_psyh_c::move(float dst_x, float dst_y, move_type_e type, bool temporary)
{
  game_ctx_t *ctx = game_ctx_get();

  if(pos_x == dst_x && pos_y == dst_y)
  {
    return RET_OK;
  }

  if(type == MOVE_TYPE_REL)
  {
    dst_x += pos_x;
    dst_y += pos_y;
  }

  // set direction
  dir = SDL_atan((dst_y - pos_y) / (dst_x - pos_x));
  if(dst_x < pos_x)
    dir += M_PI;

  // calculate destination
  float rel_x = speed * cos(dir);
  float rel_y = speed * sin(dir);
  float step_dst_x = pos_x + (rel_x * ctx->move_mult);
  float step_dst_y = pos_y + (rel_y * ctx->move_mult);
  float dist = speed * ctx->move_mult;

  if(ABS_DIST(dst_x, dst_y, step_dst_x, step_dst_y) < dist)
  {
    // destination reached
    pos_x = dst_x;
    pos_y = dst_y;

    //if(!temporary)
    //  attr_wander_pos_update((attr_wander_data_t *)unit_attr_data_get(unit, ATTR_ID_WANDER), data->pos_x, data->pos_y);

    return RET_OK;
  }

  // destination not reached
  pos_x = step_dst_x;
  pos_y = step_dst_y;
  
  return RET_PENDING;
}

void mod_psyh_c::pos_get(float *x, float *y)
{
  *x = pos_x;
  *y = pos_y;
}

void mod_psyh_c::pos_rel_to_abs(float *x, float *y)
{
  *x += pos_x;
  *y += pos_y;
}

// ============================================================= //

mod_visu_c::mod_visu_c(anim_c *anim, anim_stage_id_e stage_id, bool cycle) 
: mod_c(MOD_ID_VISU, MOD_TYPE_BASIC, MOD_LCS_RUN),
anim(anim), anim_stage_id(stage_id), anim_cycle(cycle), anim_ticks(0), visible(true)
{
}

mod_visu_c::~mod_visu_c()
{
}

void mod_visu_c::run()
{
  mod_psyh_c *psyh = (mod_psyh_c*)unit->mod_get(MOD_ID_PSYH);
  game_ctx_t *ctx = game_ctx_get();

  if(!psyh || !visible)
    return;
  
  SDL_FRect rect;
  rect.x = psyh->pos_x - (psyh->size_x / 2);
  rect.y = psyh->pos_y - (psyh->size_y / 2);
  rect.w = psyh->size_x;
  rect.h = psyh->size_y;

  anim_ticks += ctx->ticks_delta_ms;

  SDL_Texture *texture;
  texture = anim->tex_get(anim_stage_id, &anim_ticks, anim_cycle);

  if(!texture) 
    texture = anim->tex_get(ANIM_STAGE_ID_IDLE, &anim_ticks, anim_cycle);

  if(!SDL_RenderTextureRotated(ctx->renderer, texture, NULL, &rect, 
    (psyh->dir + (M_PI / 2)) * 180.0 / M_PI, NULL, SDL_FLIP_NONE))
  {
    LOG_ERROR("failed to render texture[0x%x]\n", texture);
  }

  if(unit->selected)
    gui_sel_rect_draw(
      psyh->pos_x - (psyh->size_x / 2), psyh->pos_y - (psyh->size_y / 2),
      psyh->pos_x + (psyh->size_x / 2), psyh->pos_y + (psyh->size_y / 2));
}

void mod_visu_c::anim_stage_set(anim_stage_id_e stage_id, bool cycle, bool reset)
{
  anim_stage_id = stage_id;
  anim_cycle = cycle;
  anim_ticks = reset ? 0 : anim_ticks;
}

Uint32 mod_visu_c::anim_ticks_get(anim_stage_id_e stage_id)
{
  return anim->ticks_get(stage_id);
}

// ------------------------------------------------------------- //

#if 0
void attr_wander_run(void *unit_ref, void *attr_ref)
{
  unit_t *unit = (unit_t *)unit_ref;
  attr_t *attr = (attr_t *)attr_ref;

  attr_wander_data_t *data = (attr_wander_data_t *)attr->data;
  game_ctx_t *ctx = game_ctx_get();

  if(!data->initialized)
  {
    attr_psyh_data_t *psyh_data = (attr_psyh_data_t *)unit_attr_data_get(unit, ATTR_ID_PSYH);
    if(psyh_data)
    {
      attr_psyh_pos_get(psyh_data, &data->org_x, &data->org_y);
      data->initialized = 1;
    }
    else
    {
      LOG_DEBUG("no psyh data\n");
    }
  }

  data->ticks_ms += ctx->ticks_delta_ms;
  if(data->ticks_ms > data->ticks_next_ms)
  {
    if(unit_cmd_is_empty(unit))
    {
      float dist = FRAND(data->range);
      float dir = FRAND(M_PI * 2);
      float dst_x = cos(dir) * dist;
      float dst_y = sin(dir) * dist;
      //LOG_DEBUG("attr_wander_run: wander relative x[%f] y[%f]\n", dst_x, dst_y);
      unit_attr_add(unit, attr_move_new(data->org_x + dst_x, data->org_y + dst_y, MOVE_TYPE_ABS, 1));
    }
    data->ticks_next_ms = SDL_rand(data->ticks_max_ms);
    data->ticks_ms = 0;
  }
}

void attr_wander_pos_update(attr_wander_data_t *data, float pos_x, float pos_y)
{
  data->org_x = pos_x;
  data->org_y = pos_y;
}

attr_t *attr_wander_new(float range, float ticks_max_ms)
{
  attr_wander_data_t *data = (attr_wander_data_t *)malloc(sizeof(attr_wander_data_t));
  data->range = range;
  data->ticks_max_ms = ticks_max_ms;
  data->initialized = 0;
  data->ticks_ms = 0;
  data->ticks_next_ms = SDL_rand(ticks_max_ms);
  return attr_new(ATTR_ID_WANDER, ATTR_TYPE_BASIC, ATTR_LCS_RUN, 0, data, attr_wander_run, NULL);
}
#endif