#ifndef __GCFG_H__
#define __GCFG_H__

#define STEP_MODE 0

#define APPNAME "HUMANS VS MACHINES"
#define WINX 1024
#define WINY 768
#define FWINX ((float)(WINX))
#define FWINY ((float)(WINY))
#define FPS_LIMIT "120"

typedef enum 
{
  RET_ERR = 0,
  RET_OK = 1
} ret_e;

#endif // __GCFG_H__