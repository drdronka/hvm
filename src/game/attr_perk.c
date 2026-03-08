#include <malloc.h>
#include <SDL3/SDL_render.h>
#include <math.h>

#include "gcfg.h"
#include "attr_perk.h"
#include "unit.h"
#include "attr.h"
#include "game_ctx.h"
#include "log.h"
#include "util.h"
#include "anim.h"

// ======================== LOCAL DATA ========================= //

// ======================== LOCAL FUNC ========================= //

// ======================== GLOBAL FUNC ======================== //

void attr_psyh_clean(void *unit_ref, void *attr_ref)
{
    attr_t *attr = attr_ref;
    if(attr->data) free(attr->data);
}

// ------------------------------------------------------------- //

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
  return attr_new(ATTR_ID_PSYH, ATTR_TYPE_PERK, ATTR_LCS_RUN, psyh_data, NULL, attr_psyh_clean);
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

void attr_visu_clean(void *unit_ref, void *attr_ref)
{
    attr_visu_data_t *data = attr_ref;
    if(data) free(data);
}

// ------------------------------------------------------------- //

attr_t *attr_visu_new(anim_t *anim, Uint8 anim_stage_id, Uint8 visible)
{
  attr_visu_data_t *visu_data = malloc(sizeof(attr_visu_data_t));
  visu_data->anim = anim;
  visu_data->anim_stage_id = anim_stage_id;
  visu_data->anim_ticks_ms = 0;
  visu_data->visible = visible;
  return attr_new(ATTR_ID_VISU, ATTR_TYPE_PERK, ATTR_LCS_RUN, visu_data, attr_visu_run, attr_visu_clean);
}
