#include <malloc.h>
#include <SDL3/SDL_log.h>

#include "gcfg.h"
#include "visu.h"
#include "unit.h"
#include "attr.h"
#include "psyh.h"

visu_t *visu_new()
{
  visu_t *visu = (visu_t*)malloc(sizeof(visu_t));
  memset(visu, 0, sizeof(visu_t));
  return visu;
}

void visu_func(void *ref)
{
  #if STEP_MODE
    SDL_Log("visu_func");
  #endif

  unit_t *unit = (unit_t*)ref;
  if(unit->active)
  {
    psyh_t *psyh = (psyh_t*)unit_attr_get(unit, ATTR_PSYH);
    visu_t *visu = (visu_t*)unit_attr_get(unit, ATTR_VISU);
    //SDL_Log("psyh[%x] visu[%x]", psyh, visu);
    if(psyh && visu)
    {
      SDL_FRect rect;
      rect.x = psyh->pos_x - (psyh->size_x / 2);
      rect.y = psyh->pos_y - (psyh->size_y / 2);
      rect.w = psyh->size_x;
      rect.h = psyh->size_y;
      //SDL_Log("drawing [%f][%f][%f][%f]", rect.x, rect.y, rect.w, rect.h);
      SDL_RenderTexture(visu->renderer, visu->tex, NULL, &rect);
    }
  }
}