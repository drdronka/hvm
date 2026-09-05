#pragma once

typedef enum 
{
  MOD_ID_ANY = 0,
  MOD_ID_PSYH,
  MOD_ID_VISU, 
  MOD_ID_MOVE,
  MOD_ID_WANDER,
} mod_id_e;

typedef enum
{
  MOD_TYPE_ANY = 0,
  MOD_TYPE_BASIC,
} mod_type_e;

typedef enum
{
  MOD_LCS_ANY = 0,
  MOD_LCS_RUN,
  MOD_LCS_CLEAN,
} mod_lcs_e;
