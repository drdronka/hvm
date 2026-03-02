#include <stdint.h>
#include <SDL3/SDL_render.h>

typedef enum 
{
  ATTR_NONE = 0,
  ATTR_PSYH,
  ATTR_BEHV,
  ATTR_VISU, 
} attr_type_e;

typedef struct attr 
{
  attr_type_e type;
  void *data;
  struct attr *next;
} attr_t;

typedef void (behv_f)(void *ref);

typedef struct behv
{
  behv_f *f;
} behv_t;

typedef struct psyh
{
  float pos_x;
  float pos_y;
  float vel_x;
  float vel_y;
  float size_x;
  float size_y;
} psyh_t;

typedef struct visu
{
  uint8_t visible;
  SDL_Renderer *renderer;
  SDL_Texture *tex;
} visu_t;

typedef struct unit
{
  uint8_t active;
  attr_t *attr_head;
  struct unit *next;
} unit_t;

attr_t *attr_add(attr_t **head);
void attr_del(attr_t **head, attr_t *attr);

unit_t *unit_add(unit_t **head);
void unit_del(unit_t **head, unit_t *unit);
void unit_proc(unit_t *unit);
attr_t *unit_attr_get(unit_t *unit, attr_type_e type);
attr_t *unit_attr_add(unit_t *unit);

behv_t *behv_new();
void behv_drift(void *ref);
void behv_draw(void *ref);

visu_t *visu_new();
psyh_t *psyh_new();