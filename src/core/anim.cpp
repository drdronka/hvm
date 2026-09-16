#include <malloc.h>
#include <string.h>

#include "anim.h"
#include "asset.h"
#include "log.h"

// ------------------------------------------------------------- //

anim_step_c::anim_step_c(SDL_Texture *tex, Uint32 ticks_ms)
  : tex(tex), ticks_ms(ticks_ms)
{
  LOG_DEBUG("tex[0x%x] ticks_ms[%u]\n", tex, ticks_ms);
}

anim_step_c::~anim_step_c()
{
}

ret_e anim_step_c::verify()
{
  if(!this->tex)
  {
    LOG_ERROR("NULL texture\n");
    return RET_ERR;
  }
  return RET_OK;  
}

// ------------------------------------------------------------- //

anim_stage_c::anim_stage_c(const char *name)
  : ticks_total_ms(0)
{
  this->name = (char*)malloc(strlen(name) + 1);
  memcpy(this->name, name, strlen(name) + 1);
  LOG_DEBUG("name[%s]\n", name);
}

anim_stage_c::~anim_stage_c()
{
  for(const auto& step : steps)
    delete step;
  steps.clear();
  free(name);
}

void anim_stage_c::step_add(anim_step_c *step)
{
  steps.push_back(step);
  this->ticks_total_ms += step->ticks_ms;
}

ret_e anim_stage_c::verify()
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

anim_c::anim_c(const char *name)
{
  this->name = (char*)malloc(strlen(name) + 1);
  memcpy(this->name, name, strlen(name) + 1);
  LOG_DEBUG("name[%s]\n", name);
}

anim_c::~anim_c()
{
  for(const auto& stage : stages)
    delete stage;
  stages.clear();
  free(name);
}

void anim_c::stage_add(anim_stage_c *stage)
{
  stages.push_back(stage);
}

ret_e anim_c::verify()
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

SDL_Texture *anim_c::tex_get(const char *stage_name, Uint32 *ticks_ms, bool cycle)
{
  for(const auto& stage : stages)
  {
    if(!strcmp(stage->name, stage_name))
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
  LOG_ERROR("texture not found: stage[%d] ticks_ms[%d]\n", stage_name, *ticks_ms);

  return NULL;
}

Uint32 anim_c::ticks_get(const char *stage_name)
{
  for(const auto& stage : stages)
    if(!strcmp(stage->name, stage_name))
      return stage->ticks_total_ms;

  return 0;
}

// ------------------------------------------------------------- //

anim_c *anim_get(std::vector<anim_c*> anims, const char *name)
{
  for(const auto& anim : anims)
    if(!strcmp(name, anim->name))
      return anim;

  LOG_ERROR("anim not found: name[%s]", name);

  return NULL;
}