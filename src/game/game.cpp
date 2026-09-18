#include <filesystem>
#include <iostream>
#include <fstream>
#include <math.h>

#include <SDL3/SDL_init.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_hints.h>

#include "gcfg.h"
#include "log.h"
#include "util.h"
#include "game.h"
#include "unit_basic.h"
#include "mod_basic.h"
#include "cmd_basic.h"
#include "asset.h"
#include "gui.h"
#include "json.hpp"

using json = nlohmann::json;

// ------------------------------------------------------------- //

game_c *game_c::get()
{
  static game_c game;
  return &game;
}

// ------------------------------------------------------------- //

SDL_AppResult game_c::init()
{
  LOG_INFO("%s\n", APPNAME);
  LOG_INFO("initializing\n");

  gui = gui_c::get();

  win_x = WINX;
  win_y = WINY;
  app_name = APPNAME;
  
  const char *fps_limit;
#if STEP_MODE  
  fps_limit = "1";
#else
  fps_limit = FPS_LIMIT;
#endif

  LOG_INFO("fps limit: %s\n", fps_limit);
  SDL_SetHint(SDL_HINT_MAIN_CALLBACK_RATE, fps_limit);

  LOG_INFO("initializing SDL\n");
  if(!SDL_Init(SDL_INIT_VIDEO))
  {
    LOG_ERROR("SDL: %s\n", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  LOG_INFO("creating window x[%d] y[%d]\n", win_x, win_y);
  if(!SDL_CreateWindowAndRenderer(APPNAME, win_x, win_y, 0, &window, &renderer))
  { 
    LOG_ERROR("SDL: %s\n", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  if(!assets_load())
  {
    LOG_ERROR("failed to load assets\n");
    return SDL_APP_FAILURE; 
  }

  ticks_total_ms = SDL_GetTicksNS() / 1000000;

  LOG_INFO("finished\n");

  return SDL_APP_CONTINUE;
}

// ------------------------------------------------------------- //

SDL_AppResult game_c::update()
{
  LOG_TRACE("update\n");

  ticks_update();
  gui->bg_draw();

  /* run clean functions */
  for(const auto& unit : units)
    unit->mod_clean(MOD_ID_ANY, MOD_TYPE_ANY);

  /* remove dead units */
  for(auto it = units.begin(); it != units.end();)
  {
    unit_c *unit = *it;
    if(unit->dead)
    {
      delete unit;
      it = units.erase(it);
      continue;
    }
    it++;
  }

  /* run commands */
  for(const auto& unit : units)
    unit->cmd_run();

  /* run wanderer */
  for(const auto& unit : units)
    unit->mod_run(MOD_ID_WANDER, MOD_TYPE_ANY);

  /* render units */
  for(const auto& unit : units)
    unit->mod_run(MOD_ID_VISU, MOD_TYPE_ANY);
  
  /* render gui */
  gui->render();
   
  SDL_RenderPresent(renderer);

  #if STEP_MODE
    SDL_Delay(1000);
  #endif

  return SDL_APP_CONTINUE;
}

// ------------------------------------------------------------- //

void game_c::deinit()
{
  LOG_INFO("deinitializing game\n");

  if(renderer) SDL_DestroyRenderer(renderer);
  if(window) SDL_DestroyWindow(window);

  for(const auto& tex : textures)
    delete tex;
  textures.clear();

  for(const auto& anim : anims)
    delete anim;
  anims.clear();

  for(const auto& unit : units)
    delete unit;
  units.clear();

  LOG_INFO("finished\n");
}

// ------------------------------------------------------------- //

ret_e game_c::assets_load()
{
  LOG_DEBUG("loading textures: dir[%s]\n", TEXTURES_DIR);

  for(const auto& entry : std::filesystem::directory_iterator(TEXTURES_DIR)) 
    if(entry.is_regular_file() && entry.path().extension() == ".png") 
      textures.push_back(
        new asset_tex_c(
          entry.path().stem().string().c_str(), 
          entry.path().string().c_str(), 
          renderer));

  for(const auto& tex : textures)
    if(!tex->verify())
      return RET_ERR;

  LOG_DEBUG("composing animations: file[%s]\n", ANIMS_FILE);
  std::ifstream anims_json_file(ANIMS_FILE);
  json anims_json = json::parse(anims_json_file);

  if(anims_json.size() == 0)
  {
    LOG_ERROR("failed to load json: file[%s]\n", ANIMS_FILE);
    return RET_ERR;
  }

  anim_c *anim = NULL;
  anim_stage_c *stage = NULL;
  std::string last_anim_name = std::string("");
  std::string last_stage_name = std::string("");

  for (const auto& entry : anims_json["anims"]) 
  {
    std::string anim_name = entry[0];
    std::string stage_name = entry[1];
    std::string tex_name = entry[2];
    Uint32 ticks = entry[3].get<int>();

    if(anim_name != last_anim_name)
    {
      if(anim)
        anims.push_back(anim);
      anim = new anim_c(anim_name.c_str());
    }
    
    if(stage_name != last_stage_name)
    {
      if(stage)
        anim->stage_add(stage);
      stage = new anim_stage_c(stage_name.c_str());
    }

    stage->step_add(new anim_step_c(asset_tex_get(textures, tex_name.c_str()), ticks));

    last_anim_name = anim_name;
    last_stage_name = stage_name;
  }
  anim->stage_add(stage);
  anims.push_back(anim);

  for(const auto& anim : anims)
    if(!anim->verify())
      return RET_ERR;

  return RET_OK;
}

// ------------------------------------------------------------- //

void game_c::ticks_update()
{
  Uint64 ticks_ms = SDL_GetTicksNS() / 1000000;
  ticks_delta_ms = ticks_ms - ticks_total_ms;
  ticks_total_ms = ticks_ms;
  move_mult = (float)ticks_delta_ms / 10;
}

// ------------------------------------------------------------- //

void game_c::units_move(float dst_x, float dst_y, bool clear_cmd_queue)
{
  for(const auto& unit : units)
    if(unit->selected)
    {
      mod_psyh_c *psyh = (mod_psyh_c*)unit->mod_get(MOD_ID_PSYH);
      if(psyh)
        unit->cmd_add(new cmd_move_c(dst_x, dst_y, MOVE_TYPE_ABS, false), clear_cmd_queue);
    } 
}

// ------------------------------------------------------------- //

void game_c::units_kill(bool clear_cmd_queue)
{
  for(const auto& unit : units)
    if(unit->selected)
        unit->cmd_add(new cmd_death_c(), clear_cmd_queue);
}

// ------------------------------------------------------------- //

void game_c::worm_spawn(float pos_x, float pos_y)
{
  LOG_DEBUG("spawning worm\n");
  unit_c *unit = unit_worm_new(pos_x, pos_y);
  units.push_back(unit);
  //unit_attr_add(unit, attr_enter_new()); //TBD
}



