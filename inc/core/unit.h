#pragma once

#include <SDL3/SDL_stdinc.h>

#include "unit_def.h"
#include "mod_def.h"
#include <vector>

class unit_c;

class mod_c
{
public:  
  unit_c *unit;
  mod_id_e id;
  mod_type_e type;
  mod_lcs_e lcs;

  mod_c(mod_id_e id, mod_type_e type, mod_lcs_e lcs);
  ~mod_c();
  virtual void run();
  virtual void clean();
};

class unit_c
{
public:
  bool selected;
  bool dead;
  unit_id_e id;
  std::vector<mod_c*> mods;

  unit_c(unit_id_e id);
  ~unit_c();
  void mod_add(mod_c *mod);
  mod_c *mod_get(Uint32 id);
  void mods_run(mod_id_e id, mod_type_e type);
  void mods_clean(mod_id_e id, mod_type_e type);
  void *cmd_clear();
  Uint8 cmd_is_empty();
};
