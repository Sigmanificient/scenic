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

#include "scenicos.h"

#define lengthof(s) (sizeof (s) / sizeof ((s)[0]))

extern const system_cfg CFG;

typedef struct {
    bool enable_verbose_logging;
} RuntimeOpts;

typedef int (*CommandHandler)(RuntimeOpts *options, int argc, char **argv);

typedef struct {
    const char *name;
    const char *desc;
    CommandHandler handler;
} Command;

extern const Command COMMANDS[];
extern const size_t COMMANDS_LEN;

int cmd_build(RuntimeOpts *options, int argc, char **argv);
int cmd_diff(RuntimeOpts *options, int argc, char **argv);
int cmd_gc(RuntimeOpts *options, int argc, char **argv);
int cmd_rollback(RuntimeOpts *options, int argc, char **argv);
int cmd_switch(RuntimeOpts *options, int argc, char **argv);
int cmd_validate(RuntimeOpts *options, int argc, char **argv);
