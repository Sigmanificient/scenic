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
#include <stdlib.h>
#include <string.h>

#include "all.h"
#include "arena.h"
#include "build.h"

int cmd_build(RuntimeOpts *options, int argc, char **argv)
{
    arena *a;

    (void)options;
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

