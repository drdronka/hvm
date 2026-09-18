#include <SDL3/SDL_render.h>
#include <math.h>

#include "game.h"
#include "log.h"
#include "util.h"
#include "mod_basic.h"
#include "cmd_basic.h"
#include "unit.h"
#include "anim.h"
#include "gui.h"

// ------------------------------------------------------------- //

mod_psyh_c::mod_psyh_c(
  float pos_x, float pos_y, float size_x, float size_y, float speed, float dir)
  : mod_c(MOD_ID_PSYH, MOD_TYPE_BASIC, MOD_LCS_RUN), 
  pos_x(pos_x), pos_y(pos_y), size_x(size_x), size_y(size_y), speed(speed), dir(dir)
{
}

mod_psyh_c::~mod_psyh_c()
{
}

ret_e mod_psyh_c::move(float dst_x, float dst_y, move_type_e type, bool temporary)
{
  game_c *game = game_c::get();

  if(pos_x == dst_x && pos_y == dst_y)
    return RET_OK;

  if(type == MOVE_TYPE_REL)
  {
    dst_x += pos_x;
    dst_y += pos_y;
  }

  // set direction
  dir = SDL_atan((dst_y - pos_y) / (dst_x - pos_x));
  if(dst_x < pos_x)
    dir += M_PI;

  // calculate destination
  float rel_x = speed * cos(dir);
  float rel_y = speed * sin(dir);
  float step_dst_x = pos_x + (rel_x * game->move_mult);
  float step_dst_y = pos_y + (rel_y * game->move_mult);
  float dist = speed * game->move_mult;

  if(ABS_DIST(dst_x, dst_y, step_dst_x, step_dst_y) < dist)
  {
    // destination reached
    pos_x = dst_x;
    pos_y = dst_y;

    if(!temporary)
    {
      mod_wander_c *wander = (mod_wander_c*)unit->mod_get(MOD_ID_WANDER);
      if(wander)
        wander->rebase();
    }

    return RET_OK;
  }

  // destination not reached
  pos_x = step_dst_x;
  pos_y = step_dst_y;
  
  return RET_PENDING;
}

void mod_psyh_c::pos_rel_to_abs(float *x, float *y)
{
  *x += pos_x;
  *y += pos_y;
}

// ============================================================= //

mod_visu_c::mod_visu_c(anim_c *anim, const char *stage_name, bool cycle) 
: mod_c(MOD_ID_VISU, MOD_TYPE_BASIC, MOD_LCS_RUN),
anim(anim), anim_stage_name(stage_name), anim_cycle(cycle), anim_ticks(0), visible(true)
{
}

mod_visu_c::~mod_visu_c()
{
}

void mod_visu_c::run()
{
  mod_psyh_c *psyh = (mod_psyh_c*)unit->mod_get(MOD_ID_PSYH);
  game_c *game = game_c::get();
  gui_c *gui = gui_c::get();

  if(!psyh || !visible)
    return;
  
  SDL_FRect rect;
  rect.x = psyh->pos_x - (psyh->size_x / 2);
  rect.y = psyh->pos_y - (psyh->size_y / 2);
  rect.w = psyh->size_x;
  rect.h = psyh->size_y;

  anim_ticks += game->ticks_delta_ms;

  SDL_Texture *texture;
  texture = anim->tex_get(anim_stage_name, &anim_ticks, anim_cycle);

  if(!texture) 
    texture = anim->tex_get("idle", &anim_ticks, anim_cycle);

  if(!SDL_RenderTextureRotated(game->renderer, texture, NULL, &rect, 
    (psyh->dir + (M_PI / 2)) * 180.0 / M_PI, NULL, SDL_FLIP_NONE))
  {
    LOG_ERROR("failed to render texture[0x%x]\n", texture);
  }

  if(unit->selected)
    gui->draw_sel_rect(
      psyh->pos_x - (psyh->size_x / 2), psyh->pos_y - (psyh->size_y / 2),
      psyh->pos_x + (psyh->size_x / 2), psyh->pos_y + (psyh->size_y / 2));
}

void mod_visu_c::anim_stage_set(const char *stage_name, bool cycle, bool reset)
{
  anim_stage_name = stage_name;
  anim_cycle = cycle;
  anim_ticks = reset ? 0 : anim_ticks;
}

Uint32 mod_visu_c::anim_ticks_get(const char *stage_name)
{
  return anim->ticks_get(stage_name);
}

// ------------------------------------------------------------- //

mod_wander_c::mod_wander_c(float range, Uint32 ticks_max)
  : mod_c(MOD_ID_WANDER, MOD_TYPE_BASIC, MOD_LCS_RUN), 
  range(range), ticks_max(ticks_max), initialized(false), ticks(0)
{
  ticks_next = SDL_rand(ticks_max);
}

mod_wander_c::~mod_wander_c()
{
}

void mod_wander_c::run()
{
  if(!initialized)
  {
    mod_psyh_c *psyh = (mod_psyh_c*)unit->mod_get(MOD_ID_PSYH); 
    if(psyh)
    {
      org_x = psyh->pos_x;
      org_y = psyh->pos_y;
      initialized = true;
    }
    else
    {
      LOG_ERROR("no psyh module\n");
    }
  }

  ticks += game_c::get()->ticks_delta_ms;
  if(ticks > ticks_next)
  {
    if(unit->cmd_size() == 0)
    {
      float dist = FRAND(range);
      float dir = FRAND(M_PI * 2);
      float dst_x = cos(dir) * dist;
      float dst_y = sin(dir) * dist;
      unit->cmd_add(new cmd_move_c(org_x + dst_x, org_y + dst_y, MOVE_TYPE_ABS, true), false);
    }
    ticks_next = SDL_rand(ticks_max);
    ticks = 0;
  }
}

void mod_wander_c::rebase()
{
  mod_psyh_c *psyh = (mod_psyh_c*)unit->mod_get(MOD_ID_PSYH); 
  org_x = psyh->pos_x;
  org_y = psyh->pos_y;
}
