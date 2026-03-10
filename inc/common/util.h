#ifndef __GUTIL_H__
#define __GUTIL_H__

#include <SDL3/SDL_stdinc.h>
#include <math.h>

typedef enum 
{
  RET_ERR = 0,
  RET_OK,
  RET_PENDING,
} ret_e;


#define ABS(a, b) ((a) < (b) ? (b) - (a) : (a) - (b))
#define ABS_DIST(x0, y0, x1, y1) SDL_sqrt((ABS(x0, x1) * ABS(x0, x1)) + (ABS(y0, y1) * ABS(y0, y1)))
#define IS_BETWEEN(x, a, b) ((a) < (b) ? ((x) > (a) && (x) < (b)) : ((x) > (b) && (x) < (a)))

#define FRAND(range) ((float)SDL_rand(((Sint32)(range) * (Sint32)1000)) / 1000)

#endif // __GUTIL_H__