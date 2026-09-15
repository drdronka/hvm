#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL_main.h>

#include "game.h"

game_c *game;

SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv)
{
  game = new game_c();
  return game->init();
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
  return game->update();
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
  return game->event(event);
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
  delete game;
}
