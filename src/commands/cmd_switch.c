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
#include <string.h>

#include "activate.h"
#include "arena.h"
#include "realize.h"
#include "resolve.h"
#include "scenicos.h"
#include "validate.h"

static uint32_t next_generation_number(void) {
    return 1;
}

int cmd_switch(RuntimeOpts *options, int argc, char **argv)
{
    (void)argc;
    (void)argv;
    (void)options;

    if (CFG.schema_version != SCENICOS_SCHEMA_VERSION) {
        fprintf(stderr,
                "scn: config schema version %u does not match scn %u\n",
                CFG.schema_version, SCENICOS_SCHEMA_VERSION);
        return 1;
    }

    arena *a = arena_create(1 << 20);
    if (a == NULL) {
        fprintf(stderr, "scn: arena alloc failed\n");
        return 1;
    }

    fprintf(stderr, "scn: validating config\n");
    if (!validate(&CFG)) {
        arena_destroy(a);
        return 1;
    }

    fprintf(stderr, "scn: resolving %zu packages\n", CFG.pkgs.len);
    resolved_list resolved_pkgs = {0};
    resolve_error rerr = resolve(a, &CFG, &resolved_pkgs);
    if (rerr.kind != RESOLVE_OK) {
        resolve_error_print(&rerr);
        arena_destroy(a);
        return 1;
    }

    fprintf(stderr, "scn: realizing\n");
    realize_error realerr = realize(a, &CFG.pkgs, &resolved_pkgs);
    if (realerr.kind != REALIZE_OK) {
        realize_error_print(&realerr);
        arena_destroy(a);
        return 1;
    }

    uint32_t gen = next_generation_number();
    fprintf(stderr, "scn: activating generation %u\n", gen);
    int aerr = activate(a, &CFG, &resolved_pkgs, gen);
    if (aerr != 0) {
        fprintf(stderr, "scn: activation failed: %s\n", strerror(aerr));
        arena_destroy(a);
        return 1;
    }

    fprintf(stderr, "scn: switch complete (generation %u)\n", gen);
    arena_destroy(a);
    return 0;
}

