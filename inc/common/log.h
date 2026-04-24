#ifndef __LOG_H__
#define __LOG_H__

#include <stdio.h>

#include "gcfg.h"

#define LOG_LEVEL_ERROR   (LOG_LEVEL >= 1)
#define LOG_LEVEL_WARNING (LOG_LEVEL >= 2)
#define LOG_LEVEL_INFO    (LOG_LEVEL >= 3)
#define LOG_LEVEL_DEBUG   (LOG_LEVEL >= 4)
#define LOG_LEVEL_TRACE   (LOG_LEVEL >= 5)

#define LOG_STREAM_ERR stderr

#if LOG_FORCE_STDERR
  #define LOG_STREAM_STD stderr
#else
  #define LOG_STREAM_STD stdout
#endif

#if LOG_LEVEL_ERROR
  #define LOG_ERROR(...) do { fprintf(LOG_STREAM_ERR, "ERR: %s: ", __FUNCTION__); fprintf(LOG_STREAM_ERR, __VA_ARGS__); } while(0)
#else
  #define LOG_ERROR(...)
#endif

#if LOG_LEVEL_INFO
  #define LOG_INFO(...) do { fprintf(LOG_STREAM_STD, "%s: ", __FUNCTION__); fprintf(LOG_STREAM_STD, __VA_ARGS__); } while(0)
#else
  #define LOG_INFO(...)
#endif

#if LOG_LEVEL_DEBUG
  #define LOG_DEBUG(...) do { fprintf(LOG_STREAM_STD, "%s: ", __FUNCTION__); fprintf(LOG_STREAM_STD, __VA_ARGS__); } while(0)
#else
  #define LOG_DEBUG(...)
#endif

#if LOG_LEVEL_TRACE
  #define LOG_TRACE(...) do { fprintf(LOG_STREAM_STD, "%s: ", __FUNCTION__); fprintf(LOG_STREAM_STD, __VA_ARGS__); } while(0)
#else
  #define LOG_TRACE(...)
#endif

#endif // __LOG_H__