/* Copyright (C) 2026 tonybanters (tony@tonybtw.com)

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   The GNU General Public License is contained in the file LICENSE.
*/


#ifndef SCENIC_LOGGING
    #define SCENIC_LOGGING

    #include <stdbool.h>

typedef struct {
    bool enable_verbose_logging;
    bool use_structured_logs;
    // TODO: ^ merge as flagset?
} LogSettings;

typedef enum {
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR
} LogLevel;

void log_impl(
    LogSettings *settings,
    LogLevel level,
    char const *module,
    #ifdef DEBUG_MODE
    char const *file,
    int line,
    #endif
    char const *fmt,
    ...
);

    #ifdef DEBUG_MODE
        #define LOG_DEBUG_INFO __FILE__, __LINE__,
    #else
        #define LOG_DEBUG_INFO
    #endif


    #define log(settings, lvl, mod, msg) \
        log_impl( \
            settings, lvl, mod, \
            LOG_DEBUG_INFO \
            "%s", msg)

    #define logf(settings, lvl, mod, fmt, ...) \
        log_impl( \
            settings, lvl, mod, \
            LOG_DEBUG_INFO \
            fmt, __VA_ARGS__)
#endif
