#ifndef __ANIM_H__
#define __ANIM_H__

#include "SDL3/SDL_render.h"

#include "list.h"
#include "util.h"

typedef enum 
{
  ANIM_STAGE_ID_IDLE = 0,
  ANIM_STAGE_ID_ENTER,
  ANIM_STAGE_ID_MOVE,
  ANIM_STAGE_ID_DEATH,
} anim_stage_id_e;

typedef struct anim_step
{
  SDL_Texture *tex;
  Uint32 ticks_ms;
} anim_step_t;

typedef struct anim_stage
{
  list_t *step_list;
  Uint32 ticks_total_ms;
  Uint8 id;
} anim_stage_t;

typedef struct anim
{
  char *name;
  list_t *stage_list;
} anim_t;

anim_step_t *anim_step_new(SDL_Texture *tex, Uint32 ticks_ms);
void anim_step_del(anim_step_t *step);
ret_e anim_step_verify(anim_step_t *step);
anim_stage_t *anim_stage_new(anim_stage_id_e stage_id);
void anim_stage_del(anim_stage_t *stage);
ret_e anim_stage_verify(anim_stage_t *stage);
ret_e anim_stage_add_step(anim_stage_t *stage, anim_step_t *step);
anim_t *anim_new(const char *name);
void anim_del(anim_t *anim);
ret_e anim_verify(anim_t *anim);
ret_e anim_add_stage(anim_t *anim, anim_stage_t *stage);
ret_e anim_list_verify(list_t *list);
void anim_list_destroy(list_t *list);
anim_t *anim_get(list_t *anim_list, const char *name);
SDL_Texture *anim_tex_get(anim_t *anim, anim_stage_id_e stage_id, Uint32 *ticks_ms, Uint8 rotate);
Uint32 anim_stage_ticks_get(anim_t *anim, anim_stage_id_e stage_id);

#endif // __ANIM_H__