#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_timer.h>

SDL_Window *window;
SDL_Surface *surface;

SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv)
{
  SDL_Log("HUMANS VS MACHINES");
  if(!SDL_Init(SDL_INIT_VIDEO))
  {
    SDL_Log("failed to initiaize video: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  window = SDL_CreateWindow("HUMANS VS MACHINES", 800, 600, 0);
  if(!window)
  {
    SDL_Log("failed to create window: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  surface = SDL_GetWindowSurface(window);
  if(!surface)
  {
    SDL_Log("failed to get surface: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  SDL_Delay(1000);

  SDL_DestroySurface(surface);
  surface = NULL;

  SDL_DestroyWindow(window);
  window = NULL;

  SDL_Quit();

  return SDL_APP_FAILURE;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
  return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{

}
