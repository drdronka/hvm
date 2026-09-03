#include <malloc.h>
#include <string.h>

#include "anim.h"
#include "asset.h"
#include "list.h"
#include "log.h"

// ------------------------------------------------------------- //

anim_step::anim_step(SDL_Texture *tex, Uint32 ticks_ms)
  : tex(tex), ticks_ms(ticks_ms)
{
  LOG_DEBUG("tex[0x%x] ticks_ms[%u]\n", tex, ticks_ms);
}

anim_step::~anim_step()
{
}

ret_e anim_step::verify()
{
  if(!this->tex)
  {
    LOG_ERROR("NULL texture\n");
    return RET_ERR;
  }
  return RET_OK;  
}

// ------------------------------------------------------------- //

anim_stage::anim_stage(anim_stage_id_e stage_id)
  : ticks_total_ms(0), id(stage_id)
{
  LOG_DEBUG("id[%u]\n", stage_id);
}

anim_stage::~anim_stage()
{
  steps.clear();
}

void anim_stage::add_step(anim_step *step)
{
  steps.push_back(step);
  this->ticks_total_ms += step->ticks_ms;
}

ret_e anim_stage::verify()
{
  if(!steps.size())
  {
    LOG_ERROR("no steps\n");
    return RET_ERR;
  }

  for(const auto& step : this->steps)
    if(!step->verify())
      return RET_ERR;

  return RET_OK;
}

// ------------------------------------------------------------- //

anim_obj::anim_obj(const char *name)
{
  LOG_DEBUG("name[%s]\n", name);

  this->name = (char *)malloc(strlen(name) + 1);
  strncpy(this->name, name, strlen(name) + 1);
}

anim_obj::~anim_obj()
{
  stages.clear();
  if(name) 
    free(name);
}

void anim_obj::add_stage(anim_stage *stage)
{
  stages.push_back(stage);
}

ret_e anim_obj::verify()
{
  if(!stages.size())
  {
    LOG_ERROR("no stages\n");
    return RET_ERR;
  }

  for(const auto& stage : stages)
    if(!stage->verify())
      return RET_ERR;

  return RET_OK;
}

SDL_Texture *anim_obj::get_tex(anim_stage_id_e stage_id, Uint32 *ticks_ms, bool cycle)
{
  for(const auto& stage : stages)
  {
    if(stage->id == stage_id)
    {
      if(*ticks_ms >= stage->ticks_total_ms)
        if(cycle)
          *ticks_ms = *ticks_ms % stage->ticks_total_ms;
        else
          *ticks_ms = stage->ticks_total_ms - 1;

      Uint32 curr_ticks_ms = 0;
      for(const auto& step : stage->steps)
      {
        curr_ticks_ms += step->ticks_ms;
        if(*ticks_ms < curr_ticks_ms)
          return step->tex;
      }
      
      break;
    }
  }
  LOG_ERROR("texture not found: stage id[%d] ticks_ms[%d]\n", stage_id, *ticks_ms);

  return NULL;
}

Uint32 anim_obj::get_ticks(anim_stage_id_e stage_id)
{
  for(const auto& stage : stages)
    if(stage->id == stage_id)
      return stage->ticks_total_ms;

  return 0;
}

// ------------------------------------------------------------- //

anim_obj *anim_get(std::vector<anim_obj*> anims, const char *name)
{
  for(const auto& obj : anims)
    if(!strcmp(name, obj->name))
      return obj;

  LOG_ERROR("anim not found: name[%s]", name);

  return NULL;
}