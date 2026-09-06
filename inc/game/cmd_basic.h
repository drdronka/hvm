#pragma once

#include <SDL3/SDL_render.h>

#include "unit.h"
#include "core_def.h"
#include "mod_basic.h"

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

#if 0
// ------------------------------------------------------------- //

typedef struct attr_move_data
{
  float dst_x;
  float dst_y;
  move_type_e type;
  Uint8 temporary;
  Uint8 initialized;
} attr_move_data_t;

attr_t *attr_move_new(float dst_x, float dst_y, move_type_e type, Uint8 temporary);
void attr_move_run(void *unit_ref, void *attr_ref);
void attr_move_clean(void *unit_ref, void *attr_ref);

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
