#include <malloc.h>
#include <SDL3/SDL_render.h>

#include "attr.h"
#include "attr_def.h"
#include "log.h"
#include "list.h"

// ======================== LOCAL DATA ========================= //

// ======================== LOCAL FUNC ========================= //

// ======================== GLOBAL FUNC ======================== //

attr_t *attr_new(
  attr_id_e id, 
  attr_type_e type, 
  attr_lcs_e lcs, 
  Uint8 protected,
  void *data, 
  attr_f *run, 
  attr_f *clean)
{
  LOG_DEBUG("attr_new: id[%u] type[%u] lcs[%u]\n", id, type, lcs);

  attr_t *attr = malloc(sizeof(attr_t));
  memset(attr, 0, sizeof(attr));
  attr->id = id;
  attr->type = type;
  attr->lcs = lcs;
  attr->protected = protected;
  attr->data = data;
  attr->run = run;
  attr->clean = clean;

  return attr;
}

// ------------------------------------------------------------- //

void attr_del(attr_t *attr)
{
  if(!attr)
  {
    LOG_ERROR("attr_del: NULL attr\n");
  }

  LOG_DEBUG("attr_del: id[%u] type[%u] lcs[%u]\n", attr->id, attr->type, attr->lcs);
  if(attr->data) free(attr->data);
  free(attr);
}

// ------------------------------------------------------------- //

void attr_list_destroy(list_t *list)
{
  if(!list)
  {
    LOG_ERROR("attr_list_destroy: NULL list[0x%x]\n", list);
    return;
  }
  LOG_DEBUG("attr_list_destroy: list[0x%x]\n", list);

  attr_t *attr;
  list_node_t *iter = list_iter_init(list);
  while(attr = list_iter_next(&iter))
  {
    attr_del(attr);
    list_del(list, attr);
  }
  list_destroy(list);
}