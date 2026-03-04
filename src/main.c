#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL_main.h>

#include "gcfg.h"
#include "game.h"
#include "log.h"

SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv)
{
  LOG_INFO("%s\n", APPNAME);

  return game_init();
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
  return game_update();
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
  return game_event(event);
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
  game_exit();
}
