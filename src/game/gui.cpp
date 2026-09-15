#include <SDL3/SDL_render.h>

#include "gcfg.h"
#include "log.h"
#include "gui.h"
#include "game_ctx.h"

// ------------------------------------------------------------- //

gui_c::gui_c()
{
  ctx = game_ctx_c::get();
}

gui_c::~gui_c()
{
}

void gui_c::bg_draw()
{
  SDL_SetRenderDrawColor(ctx->renderer, COLOR_BG_R, COLOR_BG_G, COLOR_BG_B, 0);
  SDL_RenderFillRect(ctx->renderer, NULL);  
}

void gui_c::draw_sel_rect(float x0, float y0, float x1, float y1)
{
  SDL_FRect rect = { x0, y0, x1 - x0, y1 - y0 };
  SDL_SetRenderDrawColor(ctx->renderer, COLOR_SEL_R, COLOR_SEL_G, COLOR_SEL_B, 0);
  SDL_RenderRect(ctx->renderer, &rect);
}

// ------------------------------------------------------------- //