#include <malloc.h>

#include "anim.h"
#include "asset.h"
#include "list.h"
#include "log.h"

// ------------------------------------------------------------- //

anim_step_t *anim_step_new(SDL_Texture *texture, Uint32 ticks_ms)
{
  anim_step_t *step = malloc(sizeof(anim_step_t));
  step->texture = texture;
  step->ticks_ms = ticks_ms;
  return step;
}

// ------------------------------------------------------------- //

anim_stage_t *anim_stage_new(anim_stage_id_e stage_id)
{
  anim_stage_t *stage = malloc(sizeof(anim_stage_t));
  memset(stage, 0, sizeof(anim_stage_t));
  stage->id = stage_id;
  stage->step_list = list_new();
  return stage;
}

// ------------------------------------------------------------- //

void anim_stage_add_step(anim_stage_t *stage, anim_step_t *step)
{
  list_add(stage->step_list, step);
  stage->ticks_total_ms += step->ticks_ms;
}

// ------------------------------------------------------------- //

anim_t *anim_new()
{
  anim_t *anim = malloc(sizeof(anim_t));
  memset(anim, 0, sizeof(anim_t));
  anim->stage_list = list_new();
  return anim;
}

// ------------------------------------------------------------- //

void anim_add_stage(anim_t *anim, anim_stage_t *stage)
{
  list_add(anim->stage_list, stage);
}

// ------------------------------------------------------------- //

SDL_Texture *anim_texture_get(anim_t *anim, anim_stage_id_e stage_id, Uint32 *ticks_ms)
{
  anim_stage_t *stage;
  list_node_t *iter = list_iter_init(anim->stage_list);
  while(stage = list_iter_next(&iter))
  {
    if(stage->id == stage_id)
    {
      if(*ticks_ms >= stage->ticks_total_ms)
        *ticks_ms = *ticks_ms % stage->ticks_total_ms;

      Uint32 curr_ticks_ms = 0;
      anim_step_t *step;
      list_node_t *iter = list_iter_init(stage->step_list);
      while(step = list_iter_next(&iter))
      {
        curr_ticks_ms += step->ticks_ms;
        if(*ticks_ms < curr_ticks_ms)
        {
          return step->texture;
        }
      }
      LOG_ERROR(
        "anim_texture_get: texture not found: stage id[%d] ticks_ms[%d]\n", 
        stage_id, *ticks_ms);
      break;
    }
  }
  return NULL;
}