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

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "scenicos.h"

#include "activate.h"
#include "all.h"
#include "arena.h"
#include "build.h"
#include "error.h"
#include "realize.h"
#include "resolve.h"
#include "validate.h"

extern const system_cfg CFG;

static uint32_t next_generation_number(void) {
    return 1;
}

// TODO: probably some kind of long_opt parser
// to have nice sub commands ux

static
int cmd_build(int argc, char **argv) {
    arena *a;

    if (argc < 3) {
        fprintf(stderr, "scn: no package to build.\n");
        return EXIT_FAILURE;
    }

    a = arena_create(1 << 20);

    if (a == NULL) {
        fprintf(stderr, "scn: arena alloc failed\n");
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < PKGS_REGISTRY_LEN; i++) {
        if (!strcmp(PKGS_REGISTRY[i]->name, argv[2])) {
            build_pkg_from_def(a, PKGS_REGISTRY[i]);
            return EXIT_SUCCESS;
        }
    }
    fprintf(stderr,
        "scn: no package named [%s] to build.\n",
        argv[2]);
    return EXIT_FAILURE;
}

static int cmd_switch(void) {
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

static int cmd_validate(void) {
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

static int cmd_rollback(void) {
    arena *a = arena_create(1 << 16);
    if (a == NULL) return 1;
    int rc = activate_rollback(a);
    arena_destroy(a);
    return rc == 0 ? 0 : 1;
}

static void usage(void) {
    fprintf(stderr,
        "usage: scn <command>\n"
        "\n"
        "commands:\n"
        "  build      build a single named package from the registry\n"
        "  switch     build and activate the current config\n"
        "  validate   check the current config without activating\n"
        "  rollback   activate the previous generation\n"
        "  diff       diff two generations (default: previous vs current)\n"
        "  gc         remove store paths not referenced by any generation\n"
    );
}

int main(int argc, char **argv) {
    if (argc < 2) { usage(); return 1; }

    const char *cmd = argv[1];

    if (strcmp(cmd, "build") == 0) return cmd_build(argc, argv);
    if (strcmp(cmd, "switch") == 0)   return cmd_switch();
    if (strcmp(cmd, "validate") == 0) return cmd_validate();
    if (strcmp(cmd, "rollback") == 0) return cmd_rollback();

    fprintf(stderr, "scn: unknown command '%s'\n", cmd);
    usage();
    return 1;
}
