#include <SDL3/SDL_render.h>
#include <SDL3/SDL_init.h>

#include "gcfg.h"
#include "gui.h"
#include "game.h"
#include "mod_basic.h"
#include "util.h"

static vob_t ui_static[] = 
{
  {"map_frame", {0, 664, 360, 360}}, // map frame
  {"map_dummy", {20, 684, 320, 320}}, // map
  {"select_frame", {360, 784, 1200, 240}}, // select frame
  {"portrait_dummy", {380, 804, 200, 200}}, // portrait
  {"stats_dummy", {600, 804, 108, 200}}, // stats
  {"action_frame", {1560, 664, 360, 360}}, // action frame
};
Uint32 ui_static_size = sizeof(ui_static) / sizeof(vob_t);

// ------------------------------------------------------------- //
gui_c *gui_c::get()
{
  static gui_c gui;
  return &gui;
}

// ------------------------------------------------------------- //

SDL_AppResult gui_c::init()
{
  game = game_c::get();

  return SDL_APP_CONTINUE;
}

// ------------------------------------------------------------- //

SDL_AppResult gui_c::event(SDL_Event *event)
{
  Uint8 exit = 0;
  const bool *keys = SDL_GetKeyboardState(NULL);

  if(event->type == SDL_EVENT_MOUSE_BUTTON_DOWN)
  {
    if(event->button.button == SDL_BUTTON_MIDDLE)
      game->worm_spawn(event->button.x, event->button.y);
    else if(event->button.button == SDL_BUTTON_LEFT)
      sel_start(event->button.x, event->button.y);
    else if(event->button.button == SDL_BUTTON_RIGHT)
      game->units_move(event->button.x, event->button.y, !keys[SDL_SCANCODE_LSHIFT]);
  }
  else if(event->type == SDL_EVENT_MOUSE_BUTTON_UP)
  {
    if(event->button.button == SDL_BUTTON_LEFT)
      sel_finish(event->button.x, event->button.y);
  }
  else if(event->type == SDL_EVENT_KEY_DOWN)
  {
    if(keys[SDL_SCANCODE_SPACE])
      game->units_kill(!keys[SDL_SCANCODE_LSHIFT]);
    
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

void gui_c::render_back()
{
  SDL_SetRenderDrawColor(game->renderer, COLOR_BG_R, COLOR_BG_G, COLOR_BG_B, 0);
  SDL_RenderFillRect(game->renderer, NULL);
}

// ------------------------------------------------------------- //

void gui_c::render_front()
{
  /* render selection rectangle */
  if(sel_en)
  {
    float mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);
    draw_sel_rect(sel_x, sel_y, mouse_x, mouse_y);
  }

  /* render static frame */
  for(Uint32 n = 0; n < ui_static_size; n++)
    SDL_RenderTexture(
      game->renderer, 
      asset_tex_get(game->textures, ui_static[n].tex_name), 
      NULL, 
      &ui_static[n].pos);
}

// ------------------------------------------------------------- //

void gui_c::sel_start(float pos_x, float pos_y)
{
  sel_x = pos_x;
  sel_y = pos_y;
  sel_en = 1;
}

// ------------------------------------------------------------- //

void gui_c::sel_finish(float pos_x, float pos_y)
{
  if(sel_en)
    if(ABS(sel_x, pos_x) < SINGLE_SEL_MARGIN && ABS(sel_y, pos_y) < SINGLE_SEL_MARGIN)
      sel_finish_single(pos_x, pos_y);
    else
      sel_finish_mult(pos_x, pos_y);
  sel_en = 0;       
}

// ------------------------------------------------------------- //

void gui_c::sel_finish_mult(float pos_x, float pos_y)
{
  for(const auto& unit : game->units)
  {
    mod_psyh_c *psyh = (mod_psyh_c*)unit->mod_get(MOD_ID_PSYH);
    if(psyh)
      unit->selected = 
        IS_BETWEEN(psyh->pos_x, sel_x, pos_x) &&
        IS_BETWEEN(psyh->pos_y, sel_y, pos_y);
  }
}

// ------------------------------------------------------------- //

void gui_c::sel_finish_single(float mouse_x, float mouse_y)
{
  bool found;
  for(const auto& unit : game->units)
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
  sel_en = 0;
}

// ------------------------------------------------------------- //

void gui_c::draw_sel_rect(float x0, float y0, float x1, float y1)
{
  SDL_FRect rect = { x0, y0, x1 - x0, y1 - y0 };
  SDL_SetRenderDrawColor(game->renderer, COLOR_SEL_R, COLOR_SEL_G, COLOR_SEL_B, 0);
  SDL_RenderRect(game->renderer, &rect);
}

// ------------------------------------------------------------- //