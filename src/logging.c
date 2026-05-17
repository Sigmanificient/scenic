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

#include "logging.h"

#include <stdarg.h>
#include <stdio.h>
#include <time.h>

static
const char *LOG_LEVEL_NAMES[] = {
    "DEBUG",
    "INFO",
    "WARNING",
    "ERROR"
};

void log_impl(
    LogSettings *settings,
    LogLevel level,
    char const *module,
#ifdef DEBUG_MODE
    char const *file,
    int line,
#endif
    char const *fmt,
    ...)
{
    char msg_buf[512];
    char const *lvl_s = LOG_LEVEL_NAMES[level];
    va_list args;

    if (
        !settings->enable_verbose_logging
        && level < LOG_INFO
    )
        return;

    va_start(args, fmt);
    vsnprintf(msg_buf, sizeof(msg_buf), fmt, args);
    va_end(args);

    unsigned long current_time = (unsigned long)time(NULL);

    if (settings->use_structured_logs) {
        fprintf(stderr,
            "{"
              "\"module\":\"%s\","
              "\"level\":\"%s\","
#ifdef DEBUG_MODE
              "\"file\":\"%s\","
              "\"line\":%d,"
#endif
              "\"time\":%lu,"
              "\"msg\":\"%s\""
            "}\n",
            module, lvl_s,
#ifdef DEBUG_MODE
            file, line,
#endif
            current_time, msg_buf
        );
    } else {
        fprintf(stderr,
#ifdef DEBUG_MODE
            "[%s:%d] "
#endif
            "[%s] %s: %s\n",
#ifdef DEBUG_MODE
            file, line,
#endif
            module, lvl_s,
            msg_buf
        );
    }
}
