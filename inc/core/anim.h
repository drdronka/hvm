#ifndef __ANIM_H__
#define __ANIM_H__

#include "SDL3/SDL_render.h"

#include "list.h"
#include "util.h"
#include <vector>

typedef enum
{
  ANIM_STAGE_ID_IDLE = 0,
  ANIM_STAGE_ID_ENTER,
  ANIM_STAGE_ID_MOVE,
  ANIM_STAGE_ID_DEATH,
} anim_stage_id_e;

class anim_step
{
public:
  SDL_Texture *tex;
  Uint32 ticks_ms;

  anim_step(SDL_Texture *tex, Uint32 ticks_ms);
  ~anim_step();
  ret_e verify();
};

class anim_stage
{
public:
  anim_stage_id_e id;
  std::vector<anim_step*> steps;
  Uint32 ticks_total_ms;

  anim_stage(anim_stage_id_e stage_id);
  ~anim_stage();
  void add_step(anim_step *step);
  ret_e verify();
};

class anim_obj
{
public:
  char *name;
  std::vector<anim_stage*> stages;

  anim_obj(const char *name);
  ~anim_obj();
  void add_stage(anim_stage *stage);
  ret_e verify();
  SDL_Texture *get_tex(anim_stage_id_e stage_id, Uint32 *ticks_ms, bool cycle);
  Uint32 get_ticks(anim_stage_id_e stage_id);
};

anim_obj *anim_get(std::vector<anim_obj*> anims, const char *name);

#endif // __ANIM_H__
