#include <malloc.h>

#include "anim.h"
#include "asset.h"
#include "list.h"
#include "log.h"

// ------------------------------------------------------------- //

anim_step_t *anim_step_new(SDL_Texture *tex, Uint32 ticks_ms)
{
  LOG_DEBUG("anim_step_new: tex[0x%x] ticks_ms[%u]\n", tex, ticks_ms);

  anim_step_t *step = malloc(sizeof(anim_step_t));
  step->tex = tex;
  step->ticks_ms = ticks_ms;

  return step;
}

// ------------------------------------------------------------- //

void anim_step_del(anim_step_t *step)
{
  if(!step)
  {
    LOG_ERROR("anim_step_del: NULL step\n");
    return;
  }
  free(step);
}

// ------------------------------------------------------------- //

ret_e anim_step_verify(anim_step_t *step)
{
  if(!step)
  {
    LOG_ERROR("anim_verify: NULL step\n");
    return RET_ERR;
  }
  if(!step->tex)
  {
    LOG_ERROR("anim_verify: NULL texture\n");
    return RET_ERR;
  }
  return RET_OK;
}

// ------------------------------------------------------------- //

anim_stage_t *anim_stage_new(anim_stage_id_e stage_id)
{
  LOG_DEBUG("anim_stage_new: id[%u]\n", stage_id);

  anim_stage_t *stage = malloc(sizeof(anim_stage_t));
  memset(stage, 0, sizeof(anim_stage_t));
  stage->id = stage_id;
  stage->step_list = list_new();

  return stage;
}

// ------------------------------------------------------------- //

void anim_stage_del(anim_stage_t *stage)
{
  if(!stage)
  {
    LOG_ERROR("anim_stage_del: NULL stage");
    return;
  }

  anim_step_t *step;
  list_node_t *iter = list_iter_init(stage->step_list);
  while(step = list_iter_next(&iter))
  {
    anim_step_del(step);
    list_del(stage->step_list, step);
  }
  list_destroy(stage->step_list);
  free(stage);
}

// ------------------------------------------------------------- //

ret_e anim_stage_verify(anim_stage_t *stage)
{
  if(!stage)
  {
    LOG_ERROR("anim_verify: NULL stage\n");
    return RET_ERR;
  }
  if(!stage->step_list)
  {
    LOG_ERROR("anim_verify: NULL step list\n");
    return RET_ERR;
  }

  anim_step_t *step;
  list_node_t *step_iter = list_iter_init(stage->step_list);
  while(step = list_iter_next(&step_iter))
    if(!anim_step_verify(step))
      return RET_ERR;

  return RET_OK;
}

// ------------------------------------------------------------- //

ret_e anim_stage_add_step(anim_stage_t *stage, anim_step_t *step)
{
  if(!stage)
  {
    LOG_ERROR("anim_stage_add_step: NULL stage");
    return RET_ERR;
  }

  list_add(stage->step_list, step);
  stage->ticks_total_ms += step->ticks_ms;

  return RET_OK;
}

// ------------------------------------------------------------- //

anim_t *anim_new(const char *name)
{
  LOG_DEBUG("anim_new: name[%s]\n", name);

  anim_t *anim = malloc(sizeof(anim_t));
  memset(anim, 0, sizeof(anim_t));
  anim->stage_list = list_new();
  anim->name = malloc(strlen(name) + 1);
  strncpy(anim->name, name, strlen(name) + 1);

  return anim;
}

// ------------------------------------------------------------- //

void anim_del(anim_t *anim)
{
  LOG_DEBUG("anim_del: anim[0x%x]\n", anim);

  if(!anim)
  {
    LOG_ERROR("anim_del: NULL anim\n");
    return;
  }
   
  anim_stage_t *stage;
  list_node_t *iter = list_iter_init(anim->stage_list);
  while(stage = list_iter_next(&iter))
  {
    anim_stage_del(stage);
    list_del(anim->stage_list, stage);
  }
  list_destroy(anim->stage_list);
  if(anim->name) free(anim->name);
  free(anim);
}
// ------------------------------------------------------------- //

ret_e anim_verify(anim_t *anim)
{
  if(!anim) 
  {
    LOG_ERROR("anim_verify: NULL anim\n");
    return RET_ERR;
  }
  if(!anim->stage_list)
  {
    LOG_ERROR("anim_verify: NULL stage list\n");
    return RET_ERR;
  }

  anim_stage_t *stage;
  list_node_t *stage_iter = list_iter_init(anim->stage_list);
  while(stage = list_iter_next(&stage_iter))
    if(!anim_stage_verify(stage))
      return RET_ERR;

  return RET_OK;
}

// ------------------------------------------------------------- //

ret_e anim_add_stage(anim_t *anim, anim_stage_t *stage)
{
  if(!anim)
  {
    LOG_ERROR("anim_add_stage: NULL anim");
    return RET_ERR;
  }

  list_add(anim->stage_list, stage);

  return RET_OK;
}

// ------------------------------------------------------------- //

ret_e anim_list_verify(list_t *list)
{
  if(!list)
  {
    LOG_ERROR("anim_list_verify: NULL list\n");
    return RET_ERR;
  }
  LOG_DEBUG("anim_verify: list[0x%x]\n", list);

  anim_t *anim;
  list_node_t *anim_iter = list_iter_init(list);
  while(anim = list_iter_next(&anim_iter))
    if(!anim_verify(anim))
      return RET_ERR;

  return RET_OK;
}

// ------------------------------------------------------------- //

void anim_list_destroy(list_t *list)
{
  if(!list)
  {
    LOG_ERROR("anim_list_destroy: NULL list\n");
    return;
  }
  LOG_DEBUG("anim_list_destroy: list[0x%x]\n", list);

  anim_t *anim;
  list_node_t *anim_iter = list_iter_init(list);
  while(anim = list_iter_next(&anim_iter))
  {
    anim_del(anim);
    list_del(list, anim);
  }
  list_destroy(list);
}

// ------------------------------------------------------------- //

anim_t *anim_get(list_t *anim_list, const char *name)
{
  anim_t *anim;
  list_node_t *iter = list_iter_init(anim_list);
  while(anim = list_iter_next(&iter))
    if(!strncmp(name, anim->name, strlen(name)))
      return anim;

  LOG_ERROR("anim_get: anim not found: name[%s]", name);

  return NULL;
}

// ------------------------------------------------------------- //

SDL_Texture *anim_tex_get(anim_t *anim, anim_stage_id_e stage_id, Uint32 *ticks_ms, Uint8 rotate)
{
  anim_stage_t *stage;
  list_node_t *iter = list_iter_init(anim->stage_list);
  while(stage = list_iter_next(&iter))
  {
    if(stage->id == stage_id)
    {
      if(*ticks_ms >= stage->ticks_total_ms)
        if(rotate)
          *ticks_ms = *ticks_ms % stage->ticks_total_ms;
        else
          *ticks_ms = stage->ticks_total_ms - 1;

      Uint32 curr_ticks_ms = 0;
      anim_step_t *step;
      list_node_t *iter = list_iter_init(stage->step_list);
      while(step = list_iter_next(&iter))
      {
        curr_ticks_ms += step->ticks_ms;
        if(*ticks_ms < curr_ticks_ms)
        {
          return step->tex;
        }
      }
      LOG_ERROR(
        "anim_texture_get: error: texture not found: stage id[%d] ticks_ms[%d]\n", 
        stage_id, *ticks_ms);
      break;
    }
  }
  return NULL;
}

// ------------------------------------------------------------- //

Uint32 anim_stage_ticks_get(anim_t *anim, anim_stage_id_e stage_id)
{
  anim_stage_t *stage;
  list_node_t *iter = list_iter_init(anim->stage_list);
  while(stage = list_iter_next(&iter))
    if(stage->id == stage_id)
      return stage->ticks_total_ms;

  return 0;
}