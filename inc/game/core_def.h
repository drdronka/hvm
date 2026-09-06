#pragma once

// ============================================================= //

typedef enum
{
  MOD_TYPE_ANY = 0,
  MOD_TYPE_BASIC,
} mod_type_e;

typedef enum 
{
  MOD_ID_ANY = 0,
  MOD_ID_PSYH,
  MOD_ID_VISU, 
  MOD_ID_WANDER,
} mod_id_e;

// ============================================================= //

typedef enum 
{
  CMD_ID_ANY = 0,
  CMD_ID_MOVE,
  CMD_ID_DEATH,
} cmd_id_e;

// ============================================================= //

typedef enum 
{
  UNIT_ID_ANY = 0,
  UNIT_ID_WORM,
} unit_id_e;

// ============================================================= //