#include <malloc.h>
#include <string.h>

#include "log.h"
#include "unit.h"
#include "unit_def.h"

// ------------------------------------------------------------- //

mod_c::mod_c(mod_id_e id, mod_type_e type, mod_lcs_e lcs)
  : id(id), type(type), lcs(lcs)
{
  unit = NULL;
}

mod_c::~mod_c()
{
}

void mod_c::run()
{
}

void mod_c::clean()
{
}

// ------------------------------------------------------------- //

unit_c::unit_c(unit_id_e id) : id(id), dead(false), selected(false)
{
  LOG_DEBUG("id[%u]\n", id);
}

unit_c::~unit_c()
{
  LOG_DEBUG("id[%u]\n", id);

  for(const auto& mod : mods)
    delete mod;
  mods.clear();
}


void unit_c::mod_add(mod_c *mod)
{
  mod->unit = this;
  mods.push_back(mod);
}

mod_c *unit_c::mod_get(Uint32 id)
{
  for(const auto& mod : mods)
    if(mod->id == id)
      return mod;

  return NULL;
}

void unit_c::mods_run(mod_id_e id, mod_type_e type)
{
  for(const auto& mod : mods)
    if(mod->lcs == MOD_LCS_RUN)
      if(id == MOD_ID_ANY || mod->id == id)
        if(type == MOD_TYPE_ANY || mod->type == type)
          mod->run();
}

void unit_c::mods_clean(mod_id_e id, mod_type_e type)
{
  for(auto it = mods.begin(); it != mods.end();)
  {
    mod_c *mod = *it;
    if(mod->lcs == MOD_LCS_CLEAN && 
      (mod->id == id || id == MOD_ID_ANY) && 
      (mod->type == type || type == MOD_TYPE_ANY))
    {
      mod->clean();
      it = mods.erase(it);
      continue;
    }
    it++;
  }
}

void *unit_c::cmd_clear()
{

}

Uint8 unit_c::cmd_is_empty()
{
    
}

// ------------------------------------------------------------- //