#ifndef __GUTIL_H__
#define __GUTIL_H__

#include <math.h>

#define RET_ERR 0
#define RET_OK 1
typedef unsigned short ret_t;

#define ABS(a, b) ((a) < (b) ? (b) - (a) : (a) - (b))
#define ABS_DIST(x0, y0, x1, y1) SDL_sqrt((ABS(x0, x1) * ABS(x0, x1)) + (ABS(y0, y1) * ABS(y0, y1)))
#define IS_BETWEEN(x, a, b) ((a) < (b) ? ((x) > (a) && (x) < (b)) : ((x) > (b) && (x) < (a)))

#endif // __GUTIL_H__