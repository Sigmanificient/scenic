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

#include "command_list.h"

#include <stdio.h>

#include "arena.h"
#include "resolve.h"
#include "scenicos.h"
#include "validate.h"

int cmd_validate(RuntimeOpts *options, int argc, char **argv)
{
    (void)argc;
    (void)argv;
    (void)options;

    if (CFG.schema_version != SCENICOS_SCHEMA_VERSION) {
        fprintf(stderr, "scn: schema version mismatch\n");
        return 1;
    }
    if (!validate(&CFG)) return 1;

    arena *a = arena_create(1 << 20);
    if (a == NULL) return 1;

    resolved_list resolved_pkgs = {0};
    resolve_error rerr = resolve(a, &CFG, &resolved_pkgs);
    if (rerr.kind != RESOLVE_OK) {
        resolve_error_print(&rerr);
        arena_destroy(a);
        return 1;
    }

    fprintf(stderr, "scn: config is valid (%zu pkgs, %zu services, %zu users)\n",
            CFG.pkgs.len, CFG.services.len, CFG.users.len);
    arena_destroy(a);
    return 0;
}
