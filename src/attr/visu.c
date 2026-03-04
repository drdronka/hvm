#include <malloc.h>
#include <SDL3/SDL_log.h>

#include "gcfg.h"
#include "visu.h"
#include "unit.h"
#include "attr.h"
#include "psyh.h"
#include "game.h"

static void run(void *ref)
{
  unit_t *unit = (unit_t*)ref;
  if(unit->active)
  {
    psyh_data_t *psyh_data = (psyh_data_t*)unit_attr_data_get(unit, ATTR_PSYH);
    visu_data_t *visu_data = (visu_data_t*)unit_attr_data_get(unit, ATTR_VISU);
    //SDL_Log("psyh[%x] visu[%x]", psyh, visu);
    if(psyh_data && visu_data)
    {
      SDL_FRect rect;
      rect.x = psyh_data->pos_x - (psyh_data->size_x / 2);
      rect.y = psyh_data->pos_y - (psyh_data->size_y / 2);
      rect.w = psyh_data->size_x;
      rect.h = psyh_data->size_y;
      //SDL_Log("drawing [%f][%f][%f][%f]", rect.x, rect.y, rect.w, rect.h);
      SDL_RenderTexture(game_context_get()->renderer, visu_data->tex, NULL, &rect);
    }
  }
}

/// GLOBAL FUNC ///

attr_t *visu_new(SDL_Texture *tex, Uint8 visible)
{
  visu_data_t *visu_data = (visu_data_t*)malloc(sizeof(visu_data_t));
  visu_data->tex = tex;
  visu_data->visible = visible;
  return attr_new(ATTR_VISU, visu_data, run);
}