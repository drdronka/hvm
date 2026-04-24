#ifndef __GCFG_H__
#define __GCFG_H__

#define STEP_MODE 0 // force 1 FPS
#define LOG_LEVEL 4 // 0 none, 1 error, 2 warning, 3 info, 4 debug, 5 trace
#define LOG_FORCE_STDERR 0

#define APPNAME "HUMANS VS MACHINES"
#define WINX 1024
#define WINY 768
#define FWINX ((float)(WINX))
#define FWINY ((float)(WINY))
#define SINGLE_SEL_MARGIN 16

#define COLOR_BG_R 0
#define COLOR_BG_G 80
#define COLOR_BG_B 80

#define COLOR_SEL_R 100
#define COLOR_SEL_G 240
#define COLOR_SEL_B 100

#if STEP_MODE
  #define FPS_LIMIT "1"
#else
  #define FPS_LIMIT "120"
#endif

#endif // __GCFG_H__