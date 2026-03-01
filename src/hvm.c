#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_render.h>

#define WINX 800
#define WINY 600
#define APPNAME "HUMANS VS MACHINES"

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Surface *char_surf;
SDL_Texture *char_tex;

int charx = 400;
int chary = 300;

void deinit()
{
  if(char_surf) SDL_DestroySurface(char_surf);
  if(window) SDL_DestroyWindow(window);
}

SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv)
{
  SDL_Log("%s", APPNAME);

  window = NULL;
  renderer = NULL;

  if(!SDL_Init(SDL_INIT_VIDEO))
  {
    SDL_Log("failed to initiaize video: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  if(!SDL_CreateWindowAndRenderer(APPNAME, WINX, WINY, 0, &window, &renderer))
  {
    SDL_Log("SDL error: %s", SDL_GetError());
  }

  char_surf = SDL_LoadPNG("./img/black_square.png");
  if(!char_surf)
  {
    SDL_Log("failed to load square: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  char_tex = SDL_CreateTextureFromSurface(renderer, char_surf);
  if(!char_tex)
  {
    SDL_Log("SDL error: %s", SDL_GetError());
  }

  return SDL_APP_CONTINUE;
}

float xpos = 0, ypos = 0;
float xmax = WINX - 32, ymax = WINY-32;
uint8_t xdir = 0, ydir = 0;
//uint16_t cntr = 0;
SDL_AppResult SDL_AppIterate(void *appstate)
{
  if(xdir == 0) { xpos++; } else { xpos--; };
  if(ydir == 0) { ypos++; } else { ypos--; };
  if(xpos == 0) xdir = 0;
  if(xpos == xmax) xdir = 1;
  if(ypos == 0) ydir = 0;
  if(ypos == ymax) ydir = 1;

  SDL_SetRenderDrawColor(renderer, 0, 200, 200, 0);
  SDL_RenderFillRect(renderer, NULL);
  SDL_FRect rect = {xpos, ypos, 32, 32};
  SDL_RenderTexture(renderer, char_tex, NULL, &rect);

  SDL_RenderPresent(renderer);

  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
  if(event->type == SDL_EVENT_QUIT)
  {
    deinit();
    SDL_Quit();
  }

  return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
  deinit();
}
