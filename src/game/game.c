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
#include "list.h"
#include "unit_impl.h"
#include "attr_basic.h"
#include "attr_cmd.h"
#include "asset.h"
#include "gui.h"

// ======================== LOCAL DATA ========================= //

static game_ctx_t *ctx = NULL;

// ======================== LOCAL FUNC ========================= //

static ret_e game_assets_load()
{
  LOG_DEBUG("game_assets_load: loading textures\n");
  ctx->tex_list = list_new();

  list_add(ctx->tex_list, asset_tex_new("square", "assets/img/black_square.png", ctx->renderer));
  list_add(ctx->tex_list, asset_tex_new("worm_enter_0", "assets/img/worm_enter_0.png", ctx->renderer));
  list_add(ctx->tex_list, asset_tex_new("worm_enter_1", "assets/img/worm_enter_1.png", ctx->renderer));
  list_add(ctx->tex_list, asset_tex_new("worm_enter_2", "assets/img/worm_enter_2.png", ctx->renderer));
  list_add(ctx->tex_list, asset_tex_new("worm_enter_3", "assets/img/worm_enter_3.png", ctx->renderer));
  list_add(ctx->tex_list, asset_tex_new("worm_idle_0", "assets/img/worm_idle_0.png", ctx->renderer));
  list_add(ctx->tex_list, asset_tex_new("worm_idle_1", "assets/img/worm_idle_1.png", ctx->renderer));
  list_add(ctx->tex_list, asset_tex_new("worm_move_0", "assets/img/worm_move_0.png", ctx->renderer));
  list_add(ctx->tex_list, asset_tex_new("worm_move_1", "assets/img/worm_move_1.png", ctx->renderer));
  list_add(ctx->tex_list, asset_tex_new("worm_death_0", "assets/img/worm_death_0.png", ctx->renderer));
  list_add(ctx->tex_list, asset_tex_new("worm_death_1", "assets/img/worm_death_1.png", ctx->renderer));
  list_add(ctx->tex_list, asset_tex_new("worm_death_2", "assets/img/worm_death_2.png", ctx->renderer));
  list_add(ctx->tex_list, asset_tex_new("worm_death_3", "assets/img/worm_death_3.png", ctx->renderer));

  if(!asset_tex_list_verify(ctx->tex_list)) 
    return RET_ERR;

  LOG_DEBUG("game_assets_load: composing animations\n");
  ctx->anim_list = list_new();

  anim_t *anim;
  anim_stage_t *stage;
  anim_step_t *step;

  anim = anim_new("worm");
  stage = anim_stage_new(ANIM_STAGE_ID_ENTER);
  anim_stage_add_step(stage, anim_step_new(asset_tex_get(ctx->tex_list, "worm_enter_0"), 25));
  anim_stage_add_step(stage, anim_step_new(asset_tex_get(ctx->tex_list, "worm_enter_1"), 25));
  anim_stage_add_step(stage, anim_step_new(asset_tex_get(ctx->tex_list, "worm_enter_2"), 25));
  anim_stage_add_step(stage, anim_step_new(asset_tex_get(ctx->tex_list, "worm_enter_3"), 25));
  anim_add_stage(anim, stage);
  stage = anim_stage_new(ANIM_STAGE_ID_IDLE);
  anim_stage_add_step(stage, anim_step_new(asset_tex_get(ctx->tex_list, "worm_idle_0"), 600));
  anim_stage_add_step(stage, anim_step_new(asset_tex_get(ctx->tex_list, "worm_idle_1"), 600));
  anim_add_stage(anim, stage);
  stage = anim_stage_new(ANIM_STAGE_ID_MOVE);
  anim_stage_add_step(stage, anim_step_new(asset_tex_get(ctx->tex_list, "worm_move_0"), 150));
  anim_stage_add_step(stage, anim_step_new(asset_tex_get(ctx->tex_list, "worm_move_1"), 150));
  anim_add_stage(anim, stage);
  stage = anim_stage_new(ANIM_STAGE_ID_DEATH);
  anim_stage_add_step(stage, anim_step_new(asset_tex_get(ctx->tex_list, "worm_death_0"), 100));
  anim_stage_add_step(stage, anim_step_new(asset_tex_get(ctx->tex_list, "worm_death_1"), 200));
  anim_stage_add_step(stage, anim_step_new(asset_tex_get(ctx->tex_list, "worm_death_2"), 60));
  anim_stage_add_step(stage, anim_step_new(asset_tex_get(ctx->tex_list, "worm_death_3"), 60));
  anim_add_stage(anim, stage);
  list_add(ctx->anim_list, anim);

  if(!anim_list_verify(ctx->anim_list))
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
  LOG_INFO("game_deinit\n");

  if(ctx->renderer) SDL_DestroyRenderer(ctx->renderer);
  if(ctx->window) SDL_DestroyWindow(ctx->window);

  asset_tex_list_destroy(ctx->tex_list);
  anim_list_destroy(ctx->anim_list);
  unit_list_destroy(ctx->unit_list);

  LOG_INFO("game_deinit: finished\n");
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
  unit_t *unit;
  list_node_t *iter = list_iter_init(ctx->unit_list);
  while(unit = list_iter_next(&iter))
  {
    attr_psyh_data_t *data = unit_attr_data_get(unit, ATTR_ID_PSYH);
    if(data)
    {
      unit->selected = 
        IS_BETWEEN(data->pos_x, ctx->sel_x, pos_x) &&
        IS_BETWEEN(data->pos_y, ctx->sel_y, pos_y);
    }
  }
}

// ------------------------------------------------------------- //

static void game_sel_single(float mouse_x, float mouse_y)
{
  Uint8 found;
  unit_t *unit;
  list_node_t *iter = list_iter_init(ctx->unit_list);
  while(unit = list_iter_next(&iter))
  {
    attr_psyh_data_t *data = unit_attr_data_get(unit, ATTR_ID_PSYH);
    if(data)
    {
      unit->selected = 
        IS_BETWEEN(mouse_x, data->pos_x - (data->size_x / 2),  data->pos_x + (data->size_x / 2)) &&
        IS_BETWEEN(mouse_y, data->pos_y - (data->size_y / 2),  data->pos_y + (data->size_y / 2))
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
  unit_t *unit;
  list_node_t *iter = list_iter_init(ctx->unit_list);
  while(unit = list_iter_next(&iter))
  {
    if(unit->selected)
    {
      attr_psyh_data_t *data = unit_attr_data_get(unit, ATTR_ID_PSYH);
      if(data)
      {
        if(clear_cmd_queue)
          unit_cmd_clear_all(unit);
        unit_attr_add(unit, attr_move_new(dst_x, dst_y, MOVE_TYPE_ABS, 0));
      }
    } 
  }
}

// ------------------------------------------------------------- //

static void game_units_kill()
{
  unit_t *unit;
  list_node_t *iter = list_iter_init(ctx->unit_list);
  while(unit = list_iter_next(&iter))
  {
    if(unit->selected)
    {
      attr_psyh_data_t *data = unit_attr_data_get(unit, ATTR_ID_PSYH);
      if(data)
      {
        unit_cmd_clear_all(unit);
        unit_attr_add(unit, attr_death_new());
      }
    } 
  }
}

// ------------------------------------------------------------- //

static void game_worm_spawn(float pos_x, float pos_y)
{
  LOG_DEBUG("game_event: spawning worm\n");
  unit_t *unit = unit_worm_new(pos_x, pos_y);
  list_add(ctx->unit_list, unit);
  unit_attr_add(unit, attr_enter_new());
}

// ======================== GLOBAL FUNC ======================== //

SDL_AppResult game_init()
{
  LOG_INFO("%s\n", APPNAME);
  LOG_INFO("game_init\n");

  game_ctx_init();
  ctx = game_ctx_get();
  ctx->win_x = WINX;
  ctx->win_y = WINY;
  ctx->app_name = APPNAME;
  ctx->unit_list = list_new();
  
  char *fps_limit;
  #if STEP_MODE  
    fps_limit = "1";
  #else
    fps_limit = FPS_LIMIT;
  #endif

  LOG_INFO("game_init: fps limit: %s\n", fps_limit);
  SDL_SetHint(SDL_HINT_MAIN_CALLBACK_RATE, fps_limit);

  LOG_INFO("game_init: initializing SDL\n");
  if(!SDL_Init(SDL_INIT_VIDEO))
  {
    LOG_ERROR("game_init: SDL: %s\n", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  LOG_INFO("game_init: creating window x[%d] y[%d]\n", ctx->win_x, ctx->win_y);
  if(!SDL_CreateWindowAndRenderer(APPNAME, ctx->win_x, ctx->win_y, 0, &ctx->window, &ctx->renderer))
  { 
    LOG_ERROR("game_init: SDL: %s\n", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  if(!game_assets_load())
  {
    LOG_ERROR("game_init: failed to load assets\n");
    return SDL_APP_FAILURE; 
  }

  ctx->ticks_total_ms = SDL_GetTicksNS() / 1000000;

  LOG_INFO("game_init: finished\n");

  return SDL_APP_CONTINUE;
}

// ------------------------------------------------------------- //

SDL_AppResult game_update()
{
  LOG_TRACE("game_update\n");

  game_ticks_update();

  gui_bg_draw();

  unit_list_attr_clean(ctx->unit_list, ATTR_ID_ANY, ATTR_TYPE_ANY);
  unit_list_remove_dead(ctx->unit_list);
  unit_list_attr_run(ctx->unit_list, ATTR_ID_ANY, ATTR_TYPE_CMD);
  unit_list_attr_run(ctx->unit_list, ATTR_ID_WANDER, ATTR_TYPE_ANY);
  unit_list_attr_run(ctx->unit_list, ATTR_ID_VISU, ATTR_TYPE_ANY);

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
  LOG_TRACE("game_event: event[%d]\n", event->type);

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
      game_units_kill();
    
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
  LOG_INFO("game_exit\n");
  game_deinit();
}
