#ifndef __ASSET_H__
#define __ASSET_H__

#include "gcfg.h"

ret_e asset_texture_load_all();
void asset_texture_free_all();
SDL_Texture *asset_texture_get(char* name);

#endif // __ASSET_H__