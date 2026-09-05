#pragma once

#include <SDL3/SDL_init.h>
#include <SDL3/SDL_events.h>

SDL_AppResult game_init();
SDL_AppResult game_update();
SDL_AppResult game_event(SDL_Event *event);
void game_exit();
