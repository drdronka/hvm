#ifndef __GAME_H__
#define __GAME_H__

#include <SDL3/SDL_init.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_render.h>

#include "gcfg.h"
#include "list.h"

SDL_AppResult game_init();
SDL_AppResult game_update();
SDL_AppResult game_event(SDL_Event *event);
void game_exit();

#endif // __GAME_H__