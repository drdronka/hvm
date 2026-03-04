#include <stdio.h>
#include <SDL3/SDL_log.h>

#include "drift.h"
#include "unit.h"
#include "psyh.h"
#include "attr.h"
#include "game.h"

static void run(void *ref)
{
  unit_t *unit = (unit_t*)ref;
  game_context_t *context = game_context_get();

  if(unit->active)
  {
    psyh_data_t *psyh_data = (psyh_data_t*)unit_attr_data_get(unit, ATTR_PSYH);
    if(psyh_data)
    {
      float mov_x = (psyh_data->vel_x * (float)context->ticks_delta_ms) / (float)1000;
      float mov_y = (psyh_data->vel_y * (float)context->ticks_delta_ms) / (float)1000;

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
}

/// GLOBAL FUNC ///

attr_t *drift_new()
{
  return attr_new(ATTR_DRIFT, NULL, run);
}
