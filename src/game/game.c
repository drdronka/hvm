#include <math.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_hints.h>

#include "gcfg.h"
#include "game.h"
#include "list.h"
#include "unit_impl.h"
#include "attr_impl.h"
#include "game_ctx.h"
#include "asset.h"
#include "log.h"
#include "util.h"

extern asset_texture_t textures[];
extern const Uint32 textures_size;

// ======================== LOCAL DATA ========================= //

static game_ctx_t *ctx = NULL;
static Uint64 last_spawn = 0;

// ======================== LOCAL FUNC ========================= //

static void deinit()
{
  LOG_INFO("game: deinitializing\n");

  asset_texture_free_all(textures, textures_size);
  if(ctx->renderer) SDL_DestroyRenderer(ctx->renderer);
  if(ctx->window) SDL_DestroyWindow(ctx->window);

  LOG_INFO("game: deinitialization finished\n");
}

// ======================== GLOBAL FUNC ======================== //

SDL_AppResult game_init()
{
  LOG_INFO("game: init");

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

  LOG_INFO("game: fps limit: %s\n", fps_limit);
  SDL_SetHint(SDL_HINT_MAIN_CALLBACK_RATE, fps_limit);

  LOG_INFO("game: initializing SDL\n");
  if(!SDL_Init(SDL_INIT_VIDEO))
  {
    LOG_ERROR("error: %s\n", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  LOG_INFO("game: creating window x[%d] y[%d]\n", ctx->win_x, ctx->win_y);
  if(!SDL_CreateWindowAndRenderer(APPNAME, ctx->win_x, ctx->win_y, 0, &ctx->window, &ctx->renderer))
  { 
    LOG_ERROR("game: error: %s\n", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  SDL_SetRenderDrawBlendMode(ctx->renderer, SDL_BLENDMODE_NONE);

  LOG_INFO("game: loading assets\n");
  if(!asset_texture_load_all(textures, textures_size, ctx->renderer))
  {
    LOG_ERROR("game: error: failed to load assets\n");
    return SDL_APP_FAILURE;
  }

  LOG_INFO("game: initialization finished\n");

  return SDL_APP_CONTINUE;
}

// ------------------------------------------------------------- //

SDL_AppResult game_update()
{
  LOG_TRACE("game: update\n");

  Uint64 ticks_ms = SDL_GetTicksNS() / 1000;
  if(ctx->ticks_total_ms == 0)
  {
    ctx->ticks_total_ms = ticks_ms;
    return SDL_APP_CONTINUE;
  }
  ctx->ticks_delta_ms = ticks_ms - ctx->ticks_total_ms;
  ctx->ticks_total_ms = ticks_ms;
  ctx->move_mult = (float)ctx->ticks_delta_ms / 10000;

  game_ctx_color_set_background();
  SDL_RenderFillRect(ctx->renderer, NULL);  

  unit_t *unit;
  list_node_t *unit_iter = list_iter_init(ctx->unit_list);
  while(unit = list_iter_next(&unit_iter))
  {
    attr_t *attr;
    list_node_t *attr_iter = list_iter_init(unit->attr_list);
    while(attr = list_iter_next(&attr_iter))
      if(attr->run) 
        attr->run((void*)unit);
  }

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

  SDL_RenderPresent(ctx->renderer);

  #if STEP_MODE
    SDL_Delay(1000);
  #endif

  return SDL_APP_CONTINUE;
}

// ------------------------------------------------------------- //

SDL_AppResult game_event(SDL_Event *event)
{
  LOG_DEBUG("game: event\n");

  Uint8 exit = 0;

  switch(event->type)
  {
    case SDL_EVENT_KEY_DOWN:
      const bool *keys = SDL_GetKeyboardState(NULL);

      if(keys[SDL_SCANCODE_ESCAPE])
        exit = 1;

      //if(keys[SDL_SCANCODE_UP])    if(ypos >= CHAR_SIZE) ypos -= CHAR_SIZE;
      //if(keys[SDL_SCANCODE_DOWN])  if(ypos < WINY - CHAR_SIZE) ypos += CHAR_SIZE;
      //if(keys[SDL_SCANCODE_LEFT])  if(xpos >= CHAR_SIZE) xpos -= CHAR_SIZE;
      //if(keys[SDL_SCANCODE_RIGHT]) if(xpos < WINX - CHAR_SIZE) xpos += CHAR_SIZE;
      break;

    case SDL_EVENT_MOUSE_BUTTON_DOWN:
      if(event->button.button == SDL_BUTTON_MIDDLE)
      {
        unit_t *unit = unit_tank_new(event->button.x, event->button.y);
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
            attr_psyh_data_t *data = unit_attr_data_get(unit, ATTR_PSYH);
            if(data)
            {
              data->dir = SDL_atan((event->button.y - data->pos_y) / (event->button.x - data->pos_x));
              data->dst_x = event->button.x;
              data->dst_y = event->button.y;
              data->moving = 1;
              if(event->button.x < data->pos_x) data->dir += M_PI;
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
              attr_psyh_data_t *data = unit_attr_data_get(unit, ATTR_PSYH);
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
              attr_psyh_data_t *data = unit_attr_data_get(unit, ATTR_PSYH);
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
  LOG_INFO("game: exit\n");
  deinit();
}
