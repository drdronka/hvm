#include <malloc.h>

#include "game_ctx.h"

// ======================== LOCAL DATA ========================= //

static game_ctx_t ctx;

// ======================== LOCAL FUNC ========================= //

// ======================== GLOBAL FUNC ======================== //

void game_ctx_init()
{
  memset(&ctx, 0, sizeof(game_ctx_t));
}

// ------------------------------------------------------------- //

game_ctx_t *game_ctx_get()
{
  return &ctx;
}

// ------------------------------------------------------------- //

// TBD recolate to gui module
void game_ctx_color_set_background()
{
  SDL_SetRenderDrawColor(ctx.renderer, 0, 80, 80, 0);
}

// ------------------------------------------------------------- //

// TBD recolate to gui module
void game_ctx_color_set_select()
{
  SDL_SetRenderDrawColor(ctx.renderer, 100, 240, 100, 0);
}