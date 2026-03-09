#ifndef __ATTR_DEF_H__
#define __ATTR_DEF_H__

typedef enum 
{
  ATTR_ID_NONE = 0,
  ATTR_ID_PSYH,
  ATTR_ID_VISU, 
  ATTR_ID_MOVE,
  ATTR_ID_WANDER,
} attr_id_e;

typedef enum
{
  ATTR_TYPE_BASIC,
  ATTR_TYPE_CMD,
} attr_type_e;

typedef enum
{
  ATTR_LCS_RUN,
  ATTR_LCS_CLEAN,
} attr_lcs_e;

#endif // __ATTR_DEF_H__