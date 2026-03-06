#include <malloc.h>
#include <SDL3/SDL_render.h>
#include <math.h>

#include "gcfg.h"
#include "attr_impl.h"
#include "unit.h"
#include "attr.h"
#include "game_ctx.h"
#include "log.h"
#include "util.h"

// ======================== LOCAL DATA ========================= //

// ======================== LOCAL FUNC ========================= //

// ======================== GLOBAL FUNC ======================== //

attr_t *attr_psyh_new(float pos_x, float pos_y, float size_x, float size_y, float speed)
{
  attr_psyh_data_t *psyh_data = malloc(sizeof(attr_psyh_data_t));
  memset(psyh_data, 0, sizeof(attr_psyh_data_t));
  psyh_data->pos_x = pos_x;
  psyh_data->pos_y = pos_y;
  psyh_data->size_x = size_x;
  psyh_data->size_y = size_y;
  psyh_data->speed = speed;
  psyh_data->dst_x;
  psyh_data->dst_y;
  return attr_new(ATTR_PSYH, psyh_data, NULL);
}

// ------------------------------------------------------------- //

void attr_visu_run(void *ref)
{
  unit_t *unit = (unit_t*)ref;
  game_ctx_t *ctx = game_ctx_get();

  attr_psyh_data_t *psyh_data = unit_attr_data_get(unit, ATTR_PSYH);
  attr_visu_data_t *visu_data = unit_attr_data_get(unit, ATTR_VISU);

  if(psyh_data && visu_data && visu_data->visible)
  {
    SDL_FRect rect;
    rect.x = psyh_data->pos_x - (psyh_data->size_x / 2);
    rect.y = psyh_data->pos_y - (psyh_data->size_y / 2);
    rect.w = psyh_data->size_x;
    rect.h = psyh_data->size_y;
    SDL_RenderTextureRotated(
      ctx->renderer, 
      visu_data->tex,
      NULL, &rect,
      (psyh_data->dir + (M_PI / 2)) * 180.0 / M_PI,
      NULL,
      SDL_FLIP_NONE);

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

attr_t *attr_visu_new(SDL_Texture *tex, Uint8 visible)
{
  attr_visu_data_t *visu_data = malloc(sizeof(attr_visu_data_t));
  visu_data->tex = tex;
  visu_data->visible = visible;
  return attr_new(ATTR_VISU, visu_data, attr_visu_run);
}

// ------------------------------------------------------------- //

void attr_drift_run(void *ref)
{
  unit_t *unit = (unit_t*)ref;
  game_ctx_t *ctx = game_ctx_get();
  attr_psyh_data_t *data = unit_attr_data_get(unit, ATTR_PSYH);

  if(data && data->moving)
  {
    float dst_x = data->pos_x + data->speed * cos(data->dir) * ctx->move_mult;
    float dst_y = data->pos_y + data->speed * sin(data->dir) * ctx->move_mult;
    float dist = data->speed * ctx->move_mult;

    if(sqrt(((data->dst_x - dst_x) * (data->dst_x - dst_x)) + ((data->dst_y - dst_y) * (data->dst_y - dst_y))) < dist)
    {
      data->pos_x = data->dst_x;
      data->pos_y = data->pos_y;
      data->moving = 0;
    }
    else
    {
      data->pos_x = dst_x;
      data->pos_y  = dst_y;
    }
    
    if(data->pos_x < 0) data->pos_x = 0 - data->pos_x;
    if(data->pos_x > FWINX) data->pos_x = FWINX - (data->pos_x - FWINX);
    if(data->pos_y < 0) data->pos_y = 0 - data->pos_y;
    if(data->pos_y > FWINY) data->pos_y = FWINY - (data->pos_y - FWINY);
  }
}

// ------------------------------------------------------------- //

attr_t *attr_drift_new()
{
  return attr_new(ATTR_DRIFT, NULL, attr_drift_run);
}

