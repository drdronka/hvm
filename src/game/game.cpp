#include <math.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_hints.h>

#include "gcfg.h"
#include "log.h"
#include "util.h"
#include "game_ctx.h"
#include "game.h"
#include "unit_basic.h"
#include "mod_basic.h"
#include "cmd_basic.h"
#include "asset.h"
#include "gui.h"

// ------------------------------------------------------------- //

static game_ctx_t *ctx = NULL;

// ------------------------------------------------------------- //

static ret_e game_assets_load()
{
  LOG_DEBUG("loading textures\n");

  ctx->textures.push_back(new asset_tex_c("square", "assets/img/black_square.png", ctx->renderer));
  ctx->textures.push_back(new asset_tex_c("worm_enter_0", "assets/img/worm_enter_0.png", ctx->renderer));
  ctx->textures.push_back(new asset_tex_c("worm_enter_1", "assets/img/worm_enter_1.png", ctx->renderer));
  ctx->textures.push_back(new asset_tex_c("worm_enter_2", "assets/img/worm_enter_2.png", ctx->renderer));
  ctx->textures.push_back(new asset_tex_c("worm_enter_3", "assets/img/worm_enter_3.png", ctx->renderer));
  ctx->textures.push_back(new asset_tex_c("worm_idle_0", "assets/img/worm_idle_0.png", ctx->renderer));
  ctx->textures.push_back(new asset_tex_c("worm_idle_1", "assets/img/worm_idle_1.png", ctx->renderer));
  ctx->textures.push_back(new asset_tex_c("worm_move_0", "assets/img/worm_move_0.png", ctx->renderer));
  ctx->textures.push_back(new asset_tex_c("worm_move_1", "assets/img/worm_move_1.png", ctx->renderer));
  ctx->textures.push_back(new asset_tex_c("worm_death_0", "assets/img/worm_death_0.png", ctx->renderer));
  ctx->textures.push_back(new asset_tex_c("worm_death_1", "assets/img/worm_death_1.png", ctx->renderer));
  ctx->textures.push_back(new asset_tex_c("worm_death_2", "assets/img/worm_death_2.png", ctx->renderer));
  ctx->textures.push_back(new asset_tex_c("worm_death_3", "assets/img/worm_death_3.png", ctx->renderer));

  for(const auto& tex : ctx->textures)
    if(!tex->verify())
      return RET_ERR;

  LOG_DEBUG("composing animations\n");
  anim_c *anim;
  anim_stage_c *stage;
  anim_step_c *step;

  anim = new anim_c("worm");
  stage = new anim_stage_c(ANIM_STAGE_ID_ENTER);
  stage->step_add(new anim_step_c(asset_tex_get(ctx->textures, "worm_enter_0"), 25));
  stage->step_add(new anim_step_c(asset_tex_get(ctx->textures, "worm_enter_1"), 25));
  stage->step_add(new anim_step_c(asset_tex_get(ctx->textures, "worm_enter_2"), 25));
  stage->step_add(new anim_step_c(asset_tex_get(ctx->textures, "worm_enter_3"), 25));
  anim->stage_add(stage);
  stage = new anim_stage_c(ANIM_STAGE_ID_IDLE);
  stage->step_add(new anim_step_c(asset_tex_get(ctx->textures, "worm_idle_0"), 600));
  stage->step_add(new anim_step_c(asset_tex_get(ctx->textures, "worm_idle_1"), 600));
  anim->stage_add(stage);
  stage = new anim_stage_c(ANIM_STAGE_ID_MOVE);
  stage->step_add(new anim_step_c(asset_tex_get(ctx->textures, "worm_move_0"), 150));
  stage->step_add(new anim_step_c(asset_tex_get(ctx->textures, "worm_move_1"), 150));
  anim->stage_add(stage);
  stage = new anim_stage_c(ANIM_STAGE_ID_DEATH);
  stage->step_add(new anim_step_c(asset_tex_get(ctx->textures, "worm_death_0"), 100));
  stage->step_add(new anim_step_c(asset_tex_get(ctx->textures, "worm_death_1"), 150));
  stage->step_add(new anim_step_c(asset_tex_get(ctx->textures, "worm_death_2"), 60));
  stage->step_add(new anim_step_c(asset_tex_get(ctx->textures, "worm_death_3"), 60));
  anim->stage_add(stage);
  ctx->anims.push_back(anim);

  for(const auto& anim : ctx->anims)
    if(!anim->verify())
      return RET_ERR;

  return RET_OK;
}

// ------------------------------------------------------------- //

static void game_ticks_update()
{
  Uint64 ticks_ms = SDL_GetTicksNS() / 1000000;
  ctx->ticks_delta_ms = ticks_ms - ctx->ticks_total_ms;
  ctx->ticks_total_ms = ticks_ms;
  ctx->move_mult = (float)ctx->ticks_delta_ms / 10;
}

// ------------------------------------------------------------- //

static void game_deinit()
{
  LOG_INFO("deinitializing game\n");

  if(ctx->renderer) SDL_DestroyRenderer(ctx->renderer);
  if(ctx->window) SDL_DestroyWindow(ctx->window);

  for(const auto& tex : ctx->textures)
    delete tex;
  ctx->textures.clear();

  for(const auto& anim : ctx->anims)
    delete anim;
  ctx->anims.clear();

  for(const auto& unit : ctx->units)
    delete unit;
  ctx->units.clear();

  LOG_INFO("finished\n");
}

// ------------------------------------------------------------- //

static void game_sel_start(float pos_x, float pos_y)
{
  ctx->sel_x = pos_x;
  ctx->sel_y = pos_y;
  ctx->sel_en = 1;
}

// ------------------------------------------------------------- //

static void game_sel_mult_finish(float pos_x, float pos_y)
{
  for(const auto& unit : ctx->units)
  {
    mod_psyh_c *psyh = (mod_psyh_c*)unit->mod_get(MOD_ID_PSYH);
    if(psyh)
      unit->selected = 
        IS_BETWEEN(psyh->pos_x, ctx->sel_x, pos_x) &&
        IS_BETWEEN(psyh->pos_y, ctx->sel_y, pos_y);
  }
}

// ------------------------------------------------------------- //

static void game_sel_single(float mouse_x, float mouse_y)
{
  bool found;
  for(const auto& unit : ctx->units)
  {
    mod_psyh_c *psyh = (mod_psyh_c*)unit->mod_get(MOD_ID_PSYH);
    if(psyh)
    {
      unit->selected = 
        IS_BETWEEN(mouse_x, psyh->pos_x - (psyh->size_x / 2),  psyh->pos_x + (psyh->size_x / 2)) &&
        IS_BETWEEN(mouse_y, psyh->pos_y - (psyh->size_y / 2),  psyh->pos_y + (psyh->size_y / 2))
        && !found;
                  
      if(unit->selected) 
        found = 1;
    }
  }
  ctx->sel_en = 0;
}

// ------------------------------------------------------------- //

static void game_sel_finish(float pos_x, float pos_y)
{
  if(ctx->sel_en)
    if(ABS(ctx->sel_x, pos_x) < SINGLE_SEL_MARGIN && ABS(ctx->sel_y, pos_y) < SINGLE_SEL_MARGIN)
      game_sel_single(pos_x, pos_y);
    else
      game_sel_mult_finish(pos_x, pos_y);
  ctx->sel_en = 0;       
}

// ------------------------------------------------------------- //

static void game_units_move(float dst_x, float dst_y, Uint8 clear_cmd_queue)
{
  for(const auto& unit : ctx->units)
  {
    if(unit->selected)
    {
      mod_psyh_c *psyh = (mod_psyh_c*)unit->mod_get(MOD_ID_PSYH);
      if(psyh)
      {
        if(clear_cmd_queue)
          unit->cmd_clear();
        //unit_attr_add(unit, attr_move_new(dst_x, dst_y, MOVE_TYPE_ABS, 0)); // TBD
      }
    } 
  }
}

// ------------------------------------------------------------- //

static void game_units_kill(Uint8 clear_cmd_queue)
{
  for(const auto& unit : ctx->units)
  {
    if(unit->selected)
    {
        if(clear_cmd_queue)
          unit->cmd_clear();
        //unit_attr_add(unit, attr_death_new()); // TBD
    } 
  }
}

// ------------------------------------------------------------- //

static void game_worm_spawn(float pos_x, float pos_y)
{
  LOG_DEBUG("spawning worm\n");
  unit_c *unit = unit_worm_new(pos_x, pos_y);
  ctx->units.push_back(unit);
  //unit_attr_add(unit, attr_enter_new()); //TBD
}

// ======================== GLOBAL FUNC ======================== //

SDL_AppResult game_init()
{
  LOG_INFO("%s\n", APPNAME);
  LOG_INFO("initializing\n");

  game_ctx_init();
  ctx = game_ctx_get();
  ctx->win_x = WINX;
  ctx->win_y = WINY;
  ctx->app_name = APPNAME;
  
  const char *fps_limit;
  #if STEP_MODE  
    fps_limit = "1";
  #else
    fps_limit = FPS_LIMIT;
  #endif

  LOG_INFO("fps limit: %s\n", fps_limit);
  SDL_SetHint(SDL_HINT_MAIN_CALLBACK_RATE, fps_limit);

  LOG_INFO("initializing SDL\n");
  if(!SDL_Init(SDL_INIT_VIDEO))
  {
    LOG_ERROR("SDL: %s\n", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  LOG_INFO("creating window x[%d] y[%d]\n", ctx->win_x, ctx->win_y);
  if(!SDL_CreateWindowAndRenderer(APPNAME, ctx->win_x, ctx->win_y, 0, &ctx->window, &ctx->renderer))
  { 
    LOG_ERROR("SDL: %s\n", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  if(!game_assets_load())
  {
    LOG_ERROR("failed to load assets\n");
    return SDL_APP_FAILURE; 
  }

  ctx->ticks_total_ms = SDL_GetTicksNS() / 1000000;

  LOG_INFO("finished\n");

  return SDL_APP_CONTINUE;
}

// ------------------------------------------------------------- //

SDL_AppResult game_update()
{
  LOG_TRACE("update\n");

  game_ticks_update();
  gui_bg_draw();

  /* run clean functions */
  for(const auto& unit : ctx->units)
    unit->mods_clean(MOD_ID_ANY, MOD_TYPE_ANY);

  /* remove dead units */
  for(auto it = ctx->units.begin(); it != ctx->units.end();)
  {
    unit_c *unit = *it;
    if(unit->dead)
    {
      LOG_ERROR("--- dead unit\n");
      delete unit;
      it = ctx->units.erase(it);
      continue;
    }
    it++;
  }

  /* run commands */
  //unit_list_attr_run(ctx->unit_list, ATTR_ID_ANY, ATTR_TYPE_CMD); // TBD

  /* run wanderer */
  for(const auto& unit : ctx->units)
    unit->mods_run(MOD_ID_WANDER, MOD_TYPE_ANY);

  /* render units */
  LOG_ERROR("> ITERATIN\n");
  for(const auto& unit : ctx->units)
  {
    LOG_ERROR("> UNIT\n");
    unit->mods_run(MOD_ID_VISU, MOD_TYPE_ANY);
  }
  
  if(ctx->sel_en)
  {
    float mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);
    gui_sel_rect_draw(ctx->sel_x, ctx->sel_y, mouse_x, mouse_y);
  }
   
  SDL_RenderPresent(ctx->renderer);

  #if STEP_MODE
    SDL_Delay(1000);
  #endif

  return SDL_APP_CONTINUE;
}

// ------------------------------------------------------------- //

SDL_AppResult game_event(SDL_Event *event)
{
  LOG_TRACE("event[%d]\n", event->type);

  Uint8 exit = 0;
  const bool *keys = SDL_GetKeyboardState(NULL);

  if(event->type == SDL_EVENT_MOUSE_BUTTON_DOWN)
  {
    if(event->button.button == SDL_BUTTON_MIDDLE)
      game_worm_spawn(event->button.x, event->button.y);
    else if(event->button.button == SDL_BUTTON_LEFT)
      game_sel_start(event->button.x, event->button.y);
    else if(event->button.button == SDL_BUTTON_RIGHT)
      game_units_move(event->button.x, event->button.y, !keys[SDL_SCANCODE_LSHIFT]);
  }
  else if(event->type == SDL_EVENT_MOUSE_BUTTON_UP)
  {
    if(event->button.button == SDL_BUTTON_LEFT)
      game_sel_finish(event->button.x, event->button.y);
  }
  else if(event->type == SDL_EVENT_KEY_DOWN)
  {
    if(keys[SDL_SCANCODE_SPACE])
      game_units_kill(!keys[SDL_SCANCODE_LSHIFT]);
    
    if(keys[SDL_SCANCODE_ESCAPE])
      exit = 1;
  }
  else if(event->type == SDL_EVENT_QUIT)
  {
    exit = 1;
  }

  if(exit)
  {
    SDL_Quit();
    return SDL_APP_SUCCESS;
  }

  return SDL_APP_CONTINUE;
}

// ------------------------------------------------------------- //

void game_exit()
{
  LOG_INFO("exit\n");
  game_deinit();
}
