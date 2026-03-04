#include <malloc.h>

#include "game_context.h"

// ======================== LOCAL DATA ========================= //

static game_context_t context;

// ======================== LOCAL FUNC ========================= //

// ======================== GLOBAL FUNC ======================== //

void game_context_init()
{
  memset(&context, 0, sizeof(game_context_t));
}

game_context_t *game_context_get()
{
  return &context;
}