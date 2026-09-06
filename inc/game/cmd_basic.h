#pragma once

#include <SDL3/SDL_render.h>

#include "unit.h"
#include "core_def.h"
#include "mod_basic.h"

// ------------------------------------------------------------- //

class cmd_move_c : public cmd_c
{
public:
  float dst_x;
  float dst_y;
  move_type_e type;
  bool temporary;
  bool initialized;
  bool finished;

  cmd_move_c(float dst_x, float dst_y, move_type_e type, bool temporary);
  ~cmd_move_c();
  void run();
};

// ------------------------------------------------------------- //

class cmd_death_c : public cmd_c
{
public:
  Uint32 ticks;
  Uint32 ticks_limit;

  cmd_death_c();
  ~cmd_death_c();
  void run();
};

// ------------------------------------------------------------- //

#if 0

// ------------------------------------------------------------- //

typedef struct attr_death_data
{
  Uint32 ticks_ms;
  Uint32 ticks_limit_ms;
} attr_death_data_t;

attr_t *attr_death_new();
void attr_death_run(void *unit_ref, void *attr_ref);
void attr_death_clean(void *unit_ref, void *attr_ref);

// ------------------------------------------------------------- //

typedef struct attr_enter_data
{
  Uint32 ticks_ms;
  Uint32 ticks_limit_ms;
} attr_enter_data_t;

attr_t *attr_enter_new();
void attr_enter_run(void *unit_ref, void *attr_ref);
void attr_enter_clean(void *unit_ref, void *attr_ref);

// ------------------------------------------------------------- //
#endif
