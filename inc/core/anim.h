#ifndef __ANIM_H__
#define __ANIM_H__

#include "SDL3/SDL_render.h"

#include "list.h"

typedef struct anim_step
{
  SDL_Texture *texture;
  Uint32 ticks_ms;
} anim_step_t;

typedef enum 
{
  ANIM_STAGE_ID_IDLE = 0,
  ANIM_STAGE_ID_INTRO,
  ANIM_STAGE_ID_MOVE,
} anim_stage_type_e;

typedef struct anim_stage
{
  list_t *step_list;
  Uint32 ticks_total_ms;
  Uint8 id; // anim_stage_id_e
} anim_stage_t;

typedef struct anim
{
  list_t *stage_list;
} anim_t;

anim_step_t *anim_step_new(SDL_Texture *texture, Uint32 ticks_ms);
anim_stage_t *anim_stage_new(Uint8 id);
void  anim_stage_add_step(anim_stage_t *stage, anim_step_t *step);
anim_t *anim_new();
void anim_add_stage(anim_t *anim, anim_stage_t *stage);
SDL_Texture *anim_texture_get(anim_t *anim, Uint8 stage_id, Uint32 *ticks_ms);

#endif // __ANIM_H__