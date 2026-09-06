#pragma once

#include <SDL3/SDL_stdinc.h>

#include "core_def.h"
#include <vector>

typedef enum
{
  MOD_LCS_ANY = 0,
  MOD_LCS_RUN,
  MOD_LCS_CLEAN,
} mod_lcs_e;

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

class cmd_c
{
public:
  unit_c *unit;
  cmd_id_e id;
  bool protect;
  
  cmd_c(cmd_id_e id);
  ~cmd_c();
  virtual void run();
};

class unit_c
{
public:
  bool selected;
  bool dead;
  unit_id_e id;
  std::vector<mod_c*> mods;
  std::vector<cmd_c*> cmds;

  unit_c(unit_id_e id);
  ~unit_c();

  void mod_add(mod_c *mod);
  mod_c *mod_get(Uint32 id);
  void mod_run(mod_id_e id, mod_type_e type);
  void mod_clean(mod_id_e id, mod_type_e type);

  void cmd_add(cmd_c *cmd);
  void cmd_run();
  void cmd_clear();
  void cmd_remove(cmd_c *cmd);
  Uint32 cmd_size();
};
