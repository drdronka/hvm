#include <SDL3/SDL_log.h>

#include "drift.h"
#include "unit.h"
#include "psyh.h"
#include "attr.h"
#include "game.h"

void drift_func(void *ref)
{
  #if STEP_MODE
    SDL_Log("drift_func");
  #endif

  unit_t *unit = (unit_t*)ref;
  game_context_t *context = game_context_get();

  if(unit->active)
  {
    psyh_t *psyh = (psyh_t*)unit_attr_get(unit, ATTR_PSYH);
    if(psyh)
    {
      #if STEP_MODE
        SDL_Log("drifting");
      #endif

      float mov_x = (psyh->vel_x * (float)context->ticks_delta_ms) / (float)1000;
      float mov_y = (psyh->vel_y * (float)context->ticks_delta_ms) / (float)1000;

      if(psyh->pos_x + mov_x < 0 || psyh->pos_x + mov_x > context->win_x)
      {
        psyh->vel_x = 0 - psyh->vel_x;
        mov_x = psyh->vel_x * (context->ticks_delta_ms / 1000);
      }
      if(psyh->pos_y + mov_y < 0 || psyh->pos_y + mov_y > context->win_y)
      {
        psyh->vel_y = 0 - psyh->vel_y;
        mov_y = psyh->vel_y * (context->ticks_delta_ms / 1000);
      }

      psyh->pos_x += mov_x;
      psyh->pos_y += mov_y;
    }
  }
}