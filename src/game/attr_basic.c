#include <malloc.h>
#include <SDL3/SDL_render.h>
#include <math.h>

#include "gcfg.h"
#include "attr_basic.h"
#include "unit.h"
#include "attr.h"
#include "game_ctx.h"
#include "log.h"
#include "util.h"
#include "anim.h"
#include "attr_cmd.h"

// ======================== LOCAL DATA ========================= //

// ======================== LOCAL FUNC ========================= //

// ======================== GLOBAL FUNC ======================== //

Uint8 attr_psyh_move(attr_psyh_data_t *data, float dst_x, float dst_y, Uint8 move_type)
{
  game_ctx_t *ctx = game_ctx_get();

  if(move_type == ATTR_PSYH_MOVE_TYPE_REL)
  {
    dst_x += data->pos_x;
    dst_y += data->pos_y;
  }

  // set direction
  data->dir = SDL_atan((dst_y - data->pos_y) / (dst_x - data->pos_x));
  if(dst_x < data->pos_x) 
    data->dir += M_PI;  

  // calculate destination
  float rel_x = data->speed * cos(data->dir);
  float rel_y = data->speed * sin(data->dir);
  float step_dst_x = data->pos_x + (rel_x * ctx->move_mult);
  float step_dst_y = data->pos_y + (rel_y * ctx->move_mult);
  float dist = data->speed * ctx->move_mult;    

  if(ABS_DIST(dst_x, dst_y, step_dst_x, step_dst_y) < dist)
  {
    // destination reached
    data->pos_x = dst_x;
    data->pos_y = dst_y;
    return ATTR_PSYH_MOVE_RET_FINISHED;
  }

  // destination not reached
  data->pos_x = step_dst_x;
  data->pos_y = step_dst_y;
  
  return ATTR_PSYH_MOVE_RET_ONGOING;
}

// ------------------------------------------------------------- //

void attr_psyh_rel_to_abs(attr_psyh_data_t *data, float *x, float *y)
{
  *x += data->pos_x;
  *y += data->pos_y;
}

// ------------------------------------------------------------- //

void attr_psyh_pos_get(attr_psyh_data_t *data, float *x, float *y)
{
  *x = data->pos_x;
  *y = data->pos_y;
}

// ------------------------------------------------------------- //

attr_t *attr_psyh_new(float pos_x, float pos_y, float size_x, float size_y, float speed, float dir)
{
  attr_psyh_data_t *psyh_data = malloc(sizeof(attr_psyh_data_t));
  memset(psyh_data, 0, sizeof(attr_psyh_data_t));
  psyh_data->pos_x = pos_x;
  psyh_data->pos_y = pos_y;
  psyh_data->size_x = size_x;
  psyh_data->size_y = size_y;
  psyh_data->speed = speed;
  psyh_data->dir = dir;
  return attr_new(ATTR_ID_PSYH, ATTR_TYPE_BASIC, ATTR_LCS_RUN, psyh_data, NULL, NULL);
}

// ============================================================= //

void attr_visu_run(void *unit_ref, void *attr_ref)
{
  unit_t *unit = unit_ref;
  attr_t *attr = attr_ref;

  game_ctx_t *ctx = game_ctx_get();
  attr_psyh_data_t *psyh_data = unit_attr_data_get(unit, ATTR_ID_PSYH);
  attr_visu_data_t *visu_data = attr->data;

  if(psyh_data && visu_data && visu_data->visible)
  {
    SDL_FRect rect;
    rect.x = psyh_data->pos_x - (psyh_data->size_x / 2);
    rect.y = psyh_data->pos_y - (psyh_data->size_y / 2);
    rect.w = psyh_data->size_x;
    rect.h = psyh_data->size_y;

    visu_data->anim_ticks_ms += ctx->ticks_delta_ms;
    SDL_Texture *texture = anim_texture_get(
      visu_data->anim, 
      visu_data->anim_stage_id, 
      &visu_data->anim_ticks_ms);

    if(!SDL_RenderTextureRotated(
      ctx->renderer, 
      texture,
      NULL, &rect,
      (psyh_data->dir + (M_PI / 2)) * 180.0 / M_PI,
      NULL,
      SDL_FLIP_NONE))
    {
      LOG_ERROR("attr_visu: failed to render texture[0x%x]\n", texture);
    }

    if(unit->selected)
    {
      game_ctx_color_set_select();
      SDL_FRect rect = {
        psyh_data->pos_x - (psyh_data->size_x / 2),
        psyh_data->pos_y - (psyh_data->size_y / 2),
        psyh_data->size_x,
        psyh_data->size_y,
      };
      SDL_RenderRect(ctx->renderer, &rect);
    }
  }
}

// ------------------------------------------------------------- //

void attr_visu_anim_stage_set(attr_visu_data_t *data)
{
  if(data) data->anim_stage_id = ANIM_STAGE_ID_MOVE;
}

// ------------------------------------------------------------- //

attr_t *attr_visu_new(anim_t *anim, Uint8 anim_stage_id, Uint8 visible)
{
  attr_visu_data_t *visu_data = malloc(sizeof(attr_visu_data_t));
  visu_data->anim = anim;
  visu_data->anim_stage_id = anim_stage_id;
  visu_data->anim_ticks_ms = 0;
  visu_data->visible = visible;
  return attr_new(ATTR_ID_VISU, ATTR_TYPE_BASIC, ATTR_LCS_RUN, visu_data, attr_visu_run, NULL);
}

// ============================================================= //

void attr_wander_run(void *unit_ref, void *attr_ref)
{
  unit_t *unit = unit_ref;
  attr_t *attr = attr_ref;

  attr_wander_data_t *data = attr->data;
  game_ctx_t *ctx = game_ctx_get();

  if(!data->initialized)
  {
    attr_psyh_data_t *psyh_data = unit_attr_data_get(unit, ATTR_ID_PSYH);
    if(psyh_data)
    {
      attr_psyh_pos_get(psyh_data, &data->org_x, &data->org_y);
      data->initialized = 1;
    }
    else
    {
      LOG_DEBUG("attr_wander_run: no psyh data\n");
    }
  }

  data->ticks_ms += ctx->ticks_delta_ms;
  if(data->ticks_ms > data->ticks_next_ms)
  {
    if(unit_cmd_is_empty(unit))
    {
      float dist = SDL_rand(data->range);
      float dir = SDL_rand(2 * M_PI);
      float dst_x = cos(dir) * dist;
      float dst_y = sin(dir) * dist;

      if(dst_x > dist) dst_x = dist; // flaot error failsafe
      if(dst_y > dist) dst_y = dist; // flaot error failsafe

      LOG_TRACE("attr_wander_run: wander relative x[%f] y[%f]\n", dst_x, dst_y);

      unit_attr_add(unit, attr_move_new(data->org_x + dst_x, data->org_y + dst_y, ATTR_PSYH_MOVE_TYPE_ABS));
    }
    data->ticks_next_ms = SDL_rand(data->ticks_max_ms);
    data->ticks_ms = 0;
  }
}

// ------------------------------------------------------------- //

void attr_wander_pos_set(attr_wander_data_t *data, float pos_x, float pos_y)
{
  data->org_x = pos_x;
  data->org_y = pos_y;
}

// ------------------------------------------------------------- //

attr_t *attr_wander_new(float range, float ticks_max_ms)
{
  attr_wander_data_t *data = malloc(sizeof(attr_wander_data_t));
  data->range = range;
  data->ticks_max_ms = ticks_max_ms;
  data->initialized = 0;
  data->ticks_ms = 0;
  data->ticks_next_ms = SDL_rand(ticks_max_ms);
  return attr_new(ATTR_ID_WANDER, ATTR_TYPE_BASIC, ATTR_LCS_RUN, data, attr_wander_run, NULL);
}

