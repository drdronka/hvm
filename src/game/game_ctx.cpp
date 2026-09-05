#include <malloc.h>
#include <string.h>

#include "game_ctx.h"

// ------------------------------------------------------------- //

static game_ctx_t ctx;

// ------------------------------------------------------------- //

void game_ctx_init()
{
  memset(&ctx, 0, sizeof(game_ctx_t));
}

game_ctx_t *game_ctx_get()
{
  return &ctx;
}

// ------------------------------------------------------------- //
