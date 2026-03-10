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

// ======================== LOCAL DATA ========================= //

static game_ctx_t *ctx = NULL;

// ======================== LOCAL FUNC ========================= //

static ret_e game_assets_load()
{
  LOG_DEBUG("game_assets_load: loading textures\n");
  ctx->tex_list = list_new();
  list_add(ctx->tex_list, asset_tex_new("worm_idle_0", "assets/img/worm_idle_0.png", ctx->renderer));
  list_add(ctx->tex_list, asset_tex_new("worm_idle_1", "assets/img/worm_idle_1.png", ctx->renderer));
  list_add(ctx->tex_list, asset_tex_new("worm_move_0", "assets/img/worm_move_0.png", ctx->renderer));
  list_add(ctx->tex_list, asset_tex_new("worm_move_1", "assets/img/worm_move_1.png", ctx->renderer));

  if(!asset_tex_list_verify(ctx->tex_list)) 
    return RET_ERR;

  LOG_DEBUG("game_assets_load: composing animations\n");
  ctx->anim_list = list_new();
  anim_t *anim_worm = anim_new("worm");
  anim_stage_t *stage_idle = anim_stage_new(ANIM_STAGE_ID_IDLE);
  anim_stage_add_step(stage_idle, anim_step_new(asset_tex_get(ctx->tex_list, "worm_idle_0"), 600));
  anim_stage_add_step(stage_idle, anim_step_new(asset_tex_get(ctx->tex_list, "worm_idle_1"), 600));
  anim_add_stage(anim_worm, stage_idle);
  anim_stage_t *stage_move = anim_stage_new(ANIM_STAGE_ID_MOVE);
  anim_stage_add_step(stage_move, anim_step_new(asset_tex_get(ctx->tex_list, "worm_move_0"), 150));
  anim_stage_add_step(stage_move, anim_step_new(asset_tex_get(ctx->tex_list, "worm_move_1"), 150));
  anim_add_stage(anim_worm, stage_move);
  list_add(ctx->anim_list, anim_worm);

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

static void game_back_draw()
{
  game_ctx_color_set_background();
  SDL_RenderFillRect(ctx->renderer, NULL);  
}

// ------------------------------------------------------------- //

static void game_sel_rect_draw()
{
  if(ctx->sel_en)
  {
    float mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);
    game_ctx_color_set_select();
    SDL_RenderLine(ctx->renderer, mouse_x, mouse_y, ctx->sel_x, mouse_y);
    SDL_RenderLine(ctx->renderer, mouse_x, mouse_y, mouse_x, ctx->sel_y);
    SDL_RenderLine(ctx->renderer, ctx->sel_x, mouse_y, ctx->sel_x, ctx->sel_y);
    SDL_RenderLine(ctx->renderer, mouse_x, ctx->sel_y, ctx->sel_x, ctx->sel_y);
  }
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

// ======================== GLOBAL FUNC ======================== //

SDL_AppResult game_init()
{
  LOG_INFO("game_init");

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

  LOG_INFO("game_init: initialization finished\n");

  return SDL_APP_CONTINUE;
}

// ------------------------------------------------------------- //

SDL_AppResult game_update()
{
  LOG_TRACE("game_update\n");

  game_ticks_update();
  game_back_draw();
  unit_list_attr_clean(ctx->unit_list, ATTR_ID_ANY, ATTR_TYPE_ANY);
  unit_list_attr_run(ctx->unit_list, ATTR_ID_ANY, ATTR_TYPE_CMD);
  unit_list_attr_run(ctx->unit_list, ATTR_ID_WANDER, ATTR_TYPE_ANY);
  unit_list_attr_run(ctx->unit_list, ATTR_ID_VISU, ATTR_TYPE_ANY);
  game_sel_rect_draw();

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

  switch(event->type)
  {
    case SDL_EVENT_KEY_DOWN:
      if(keys[SDL_SCANCODE_ESCAPE])
        exit = 1;
      break;

    case SDL_EVENT_MOUSE_BUTTON_DOWN:
      if(event->button.button == SDL_BUTTON_MIDDLE)
      {
        LOG_DEBUG("game_event: adding unit\n");
        unit_t *unit = unit_worm_new(event->button.x, event->button.y);
        list_add(ctx->unit_list, unit);
      }
      else if(event->button.button == SDL_BUTTON_LEFT)
      {
        if(!ctx->sel_en)
        {
          ctx->sel_x = event->button.x;
          ctx->sel_y = event->button.y;
          ctx->sel_en = 1;
        }
      }
      else if(event->button.button == SDL_BUTTON_RIGHT)
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
              if(!keys[SDL_SCANCODE_LSHIFT])
                unit_cmd_clear_all(unit);
              unit_attr_add(
                unit, attr_move_new(event->button.x, event->button.y, MOVE_TYPE_ABS, 0));
            }
          } 
        }
      }
      break;

    case SDL_EVENT_MOUSE_BUTTON_UP:
      if(event->button.button == SDL_BUTTON_LEFT)
      {
        if(ctx->sel_en)
        {
          // single selection
          if(ABS(ctx->sel_x, event->button.x) < SINGLE_SEL_MARGIN && 
            ABS(ctx->sel_y, event->button.y) < SINGLE_SEL_MARGIN)
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
                  IS_BETWEEN(
                    event->button.x, 
                    data->pos_x - (data->size_x / 2), 
                    data->pos_x + (data->size_x / 2)) 
                  && IS_BETWEEN(
                    event->button.y,
                    data->pos_y - (data->size_y / 2),
                    data->pos_y + (data->size_y / 2))
                  && !found;
                  
                if(unit->selected) 
                  found = 1;
              }
            }
          }
          else
          {
            unit_t *unit;
            list_node_t *iter = list_iter_init(ctx->unit_list);
            while(unit = list_iter_next(&iter))
            {
              attr_psyh_data_t *data = unit_attr_data_get(unit, ATTR_ID_PSYH);
              if(data)
              {
                unit->selected = 
                  IS_BETWEEN(data->pos_x, ctx->sel_x, event->button.x) &&
                  IS_BETWEEN(data->pos_y, ctx->sel_y, event->button.y);
              }
            }
          }
          ctx->sel_en = 0;
        }
      }
      break;

    case SDL_EVENT_QUIT:
      exit = 1;
      break;
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
