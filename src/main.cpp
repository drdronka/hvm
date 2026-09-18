#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL_main.h>

#include "game.h"
#include "gui.h"

game_c *game;
gui_c *gui;

SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv)
{
  game = game_c::get();
  gui = gui_c::get();

  if(game->init() != SDL_APP_CONTINUE || gui->init() != SDL_APP_CONTINUE)
    return SDL_APP_FAILURE;

  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
  return game->update();
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
  return gui->event(event);
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
  game->deinit();
}
