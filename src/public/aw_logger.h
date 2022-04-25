//
// Created by pengc on 2022/4/24.
//

#ifndef INCLUDED_AW_LOGGER_H
#define INCLUDED_AW_LOGGER_H

#ifndef INCLUDED_STDARG_H
#define INCLUDED_STDARG_H
#include <stdarg.h>
#endif // INCLUDED_STDARG_H

////////////////////////////////////////////////////////////////////////////////
////
typedef enum eAwLoggerLevel{
    kAW_LOGGER_OFF,
    kAW_LOGGER_TRACE,
    kAW_LOGGER_DEBUG,
    kAW_LOGGER_INFO,
    kAW_LOGGER_WARN,
    kAW_LOGGER_ERROR,
    kAW_LOGGER_FATAL,
    kAW_LOGGER_ALL
}eAwLoggerLevel;

typedef struct aw_logger_t aw_logger_t;

typedef struct aw_logger_ops{
    const char* file;
    eAwLoggerLevel level;
}aw_logger_ops;

////////////////////////////////////////////////////////////////////////////////
////

aw_logger_t *aw_logger_new(aw_logger_ops ops);

void aw_logger_free(aw_logger_t** logger);

eAwLoggerLevel aw_logger_level(aw_logger_t * logger, eAwLoggerLevel newLevel);
    // set the new logger level and return the old level

void aw_logger_log(aw_logger_t *logger, eAwLoggerLevel level, const char* function, const char* file, int len, const char* fmt, ...);

void aw_logger_start(aw_logger_t *logger);

void aw_logger_try_stop(aw_logger_t *logger);

////////////////////////////////////////////////////////////////////////////////
////
#define AW_LOGGER_TRACE(logger, fmt, ...) \
    aw_logger_log((logger), kAW_LOGGER_TRACE, __FUNCTION__, __FILE__, __LINE__, (fmt), __VA_ARGS__)

#define AW_LOGGER_DEBUG(logger, fmt, ...) \
    aw_logger_log((logger), kAW_LOGGER_DEBUG, __FUNCTION__, __FILE__, __LINE__, (fmt), __VA_ARGS__)

#define AW_LOGGER_INFO(logger, fmt, ...) \
    aw_logger_log((logger), kAW_LOGGER_INFO, __FUNCTION__, __FILE__, __LINE__, (fmt), __VA_ARGS__)

#define AW_LOGGER_WARN(logger, fmt, ...) \
    aw_logger_log((logger), kAW_LOGGER_WARN, __FUNCTION__, __FILE__, __LINE__, (fmt), __VA_ARGS__)

#define AW_LOGGER_ERROR(logger, fmt, ...) \
    aw_logger_log((logger), kAW_LOGGER_ERROR, __FUNCTION__, __FILE__, __LINE__, (fmt), __VA_ARGS__)

#define AW_LOGGER_FATAL(logger, fmt, ...) \
    aw_logger_log((logger), kAW_LOGGER_FATAL, __FUNCTION__, __FILE__, __LINE__, (fmt), __VA_ARGS__)

#endif //INCLUDED_AW_LOGGER_H
