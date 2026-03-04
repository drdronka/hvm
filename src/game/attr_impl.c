#include <malloc.h>
#include <SDL3/SDL_render.h>

#include "attr_impl.h"
#include "unit.h"
#include "attr.h"
#include "game_context.h"

// ======================== LOCAL DATA ========================= //

// ======================== LOCAL FUNC ========================= //

// ======================== GLOBAL FUNC ======================== //

attr_t *attr_psyh_new(float pos_x, float pos_y, float vel_x, float vel_y, float size_x, float size_y)
{
  attr_psyh_data_t *psyh_data = (attr_psyh_data_t*)malloc(sizeof(attr_psyh_data_t));
  memset(psyh_data, 0, sizeof(attr_psyh_data_t));
  psyh_data->pos_x = pos_x;
  psyh_data->pos_y = pos_y;
  psyh_data->vel_x = vel_x;
  psyh_data->vel_y = vel_y;
  psyh_data->size_x = size_x;
  psyh_data->size_y = size_y;
  return attr_new(ATTR_PSYH, psyh_data, NULL);
}

// ------------------------------------------------------------- //

void attr_visu_run(void *ref)
{
  unit_t *unit = (unit_t*)ref;

  attr_psyh_data_t *psyh_data = unit_attr_data_get(unit, ATTR_PSYH);
  attr_visu_data_t *visu_data = unit_attr_data_get(unit, ATTR_VISU);

  if(psyh_data && visu_data && visu_data->visible)
  {
    SDL_FRect rect;
    rect.x = psyh_data->pos_x - (psyh_data->size_x / 2);
    rect.y = psyh_data->pos_y - (psyh_data->size_y / 2);
    rect.w = psyh_data->size_x;
    rect.h = psyh_data->size_y;
    SDL_RenderTexture(game_context_get()->renderer, visu_data->tex, NULL, &rect);
  }
}

// ------------------------------------------------------------- //

attr_t *attr_visu_new(SDL_Texture *tex, Uint8 visible)
{
  attr_visu_data_t *visu_data = (attr_visu_data_t*)malloc(sizeof(attr_visu_data_t));
  visu_data->tex = tex;
  visu_data->visible = visible;
  return attr_new(ATTR_VISU, visu_data, attr_visu_run);
}

// ------------------------------------------------------------- //

void attr_drift_run(void *ref)
{
  unit_t *unit = (unit_t*)ref;
  game_context_t *context = game_context_get();
  attr_psyh_data_t *psyh_data = unit_attr_data_get(unit, ATTR_PSYH);
  
  if(psyh_data)
  {
    float mov_x = (psyh_data->vel_x * (float)context->ticks_delta_ms) / (float)10000;
    float mov_y = (psyh_data->vel_y * (float)context->ticks_delta_ms) / (float)10000;

    if(psyh_data->pos_x + mov_x < 0 || psyh_data->pos_x + mov_x > context->win_x)
    {
      psyh_data->vel_x = 0 - psyh_data->vel_x;
      mov_x = psyh_data->vel_x * (context->ticks_delta_ms / 1000);
    }
    if(psyh_data->pos_y + mov_y < 0 || psyh_data->pos_y + mov_y > context->win_y)
    {
      psyh_data->vel_y = 0 - psyh_data->vel_y;
      mov_y = psyh_data->vel_y * (context->ticks_delta_ms / 1000);
    }

    psyh_data->pos_x += mov_x;
    psyh_data->pos_y += mov_y;
  }
}

// ------------------------------------------------------------- //

attr_t *attr_drift_new()
{
  return attr_new(ATTR_DRIFT, NULL, attr_drift_run);
}
