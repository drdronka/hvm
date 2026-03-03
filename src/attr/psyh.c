#include <malloc.h>
#include <string.h>

#include "psyh.h""

psyh_t *psyh_new()
{
  psyh_t *psyh = (psyh_t*)malloc(sizeof(psyh_t));
  memset(psyh, 0, sizeof(psyh_t));
  return psyh;
}