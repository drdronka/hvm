#ifndef __GCFG_H__
#define __GCFG_H__

#define STEP_MODE 1 // force 1 FPS
#define LOG_LEVEL 3 // 0 none, 1 error, 2 info, 3 debug, 4 trace
#define LOG_FORCE_STDERR 0

#define APPNAME "HUMANS VS MACHINES"
#define WINX 1024
#define WINY 768
#define FWINX ((float)(WINX))
#define FWINY ((float)(WINY))
#define SINGLE_SEL_MARGIN 16

#if STEP_MODE
  #define FPS_LIMIT "1"
#else
  #define FPS_LIMIT "120"
#endif

#endif // __GCFG_H__