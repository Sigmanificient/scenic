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

#include "arena.h"
#include "activate.h"

#include "command_list.h"

int cmd_rollback(RuntimeOpts *options, int argc, char **argv)
{
    (void)argc;
    (void)argv;
    (void)options;

    arena *a = arena_create(1 << 16);
    if (a == NULL) return 1;
    int rc = activate_rollback(a);
    arena_destroy(a);
    return rc == 0 ? 0 : 1;
}
