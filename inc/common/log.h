#ifndef __LOG_H__
#define __LOG_H__

#include <stdio.h>

#include "gcfg.h"

#if LOG_LEVEL > 0
  #define LOG_LEVEL_ERROR 1
#else
  #define LOG_LEVEL_ERROR 0
#endif

#if LOG_LEVEL > 1
  #define LOG_LEVEL_INFO 1
#else
  #define LOG_LEVEL_INFO 0
#endif

#if LOG_LEVEL > 2
  #define LOG_LEVEL_DEBUG 1
#else
  #define LOG_LEVEL_DEBUG 0
#endif

#if LOG_LEVEL > 3
  #define LOG_LEVEL_TRACE 1
#else
  #define LOG_LEVEL_TRACE 0
#endif

#if LOG_FORCE_STDERR

  #if LOG_LEVEL_ERROR
    #define LOG_ERROR(...) fprintf(stderr, __VA_ARGS__)
  #else
    #define LOG_ERROR(...)
  #endif

  #if LOG_LEVEL_INFO
    #define LOG_INFO(...) fprintf(stderr, __VA_ARGS__)
  #else
    #define LOG_INFO(...)
  #endif

  #if LOG_LEVEL_DEBUG
    #define LOG_DEBUG(...) fprintf(stderr, __VA_ARGS__)
  #else
    #define LOG_DEBUG(...)
  #endif

  #if LOG_LEVEL_TRACE
    #define LOG_TRACE(...) fprintf(stderr, __VA_ARGS__)
  #else
    #define LOG_TRACE(...)
  #endif

#else // LOG_FORCE_STDERR

  #if LOG_LEVEL_ERROR
    #define LOG_ERROR(...) fprintf(stderr, __VA_ARGS__)
  #else
    #define LOG_ERROR(...)
  #endif

  #if LOG_LEVEL_INFO
    #define LOG_INFO(...) printf(__VA_ARGS__)
  #else
    #define LOG_INFO(...)
  #endif

  #if LOG_LEVEL_DEBUG
    #define LOG_DEBUG(...) printf(__VA_ARGS__)
  #else
    #define LOG_DEBUG(...)
  #endif

  #if LOG_LEVEL_TRACE
    #define LOG_TRACE(...) printf(__VA_ARGS__)
  #else
    #define LOG_TRACE(...)
  #endif

#endif // __LOG_FORCE_STDERR

#endif // __LOG_H__