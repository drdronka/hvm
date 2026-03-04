#ifndef __GCFG_H__
#define __GCFG_H__

#define STEP_MODE 0 // force 1 FPS
#define LOG_LEVEL 2 // 0 none, 1 error, 2 info, 3 debug, 4 trace
#define LOG_FORCE_STDERR 0

#define APPNAME "HUMANS VS MACHINES"
#define WINX 1024
#define WINY 768
#define FWINX ((float)(WINX))
#define FWINY ((float)(WINY))

#if STEP_MODE
  #define FPS_LIMIT "1"
#else
  #define FPS_LIMIT "120"
#endif

#define RET_ERR 0
#define RET_OK 1
typedef unsigned short ret_t;

#endif // __GCFG_H__