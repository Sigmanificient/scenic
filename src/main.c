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

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "scenicos.h"

#include "command_list.h"

static
char *GLOBAL_OPTIONS_DESC[] = {
    "increase verbosity of logs",
    "show this message"
};

static
struct option GLOBAL_OPTIONS[] = {
    {"verbose", no_argument,       0, 'l'},
    {"help",    no_argument,       0, 'h'},
    {0, 0, 0, 0}
};

const Command COMMANDS[] = {
    {
        .name = "build",
        .desc = "build a single named package from the registry",
        .handler = &cmd_build
    },
    {
        .name = "switch",
        .desc = "build and activate the current config",
        .handler = &cmd_switch
    },
    {
        .name = "validate",
        .desc = "check the current config without activating",
        .handler = &cmd_validate
    },
    {
        .name = "rollback",
        .desc = "activate the previous generation",
        .handler = &cmd_rollback,
    },
    {
        .name = "diff",
        .desc = "diff two generations (default: previous vs current)",
        .handler = &cmd_diff,
    },
    {
        .name = "gc",
        .desc = "remove store paths not referenced by any generation",
        .handler = &cmd_gc
    },
};

const size_t COMMANDS_LEN = lengthof(COMMANDS);


static void print_global_usage(void)
{
    size_t longest = 0;
    size_t len;
    int padding;

    fprintf(stderr, "usage: scn [options] <command> [<args>]\n");

    for (size_t i = 0; i < COMMANDS_LEN; i++) {
        len = strlen(COMMANDS[i].name);
        if (len > longest)
            longest = len;
    }

    for (size_t i = 0; GLOBAL_OPTIONS[i].name != NULL; i++) {
        len = lengthof("-x, --") - 1 + strlen(GLOBAL_OPTIONS[i].name);
        if (len > longest)
            longest = len;
    }

    padding = (int)(longest + 2);

    fprintf(stderr, "\noptions:\n");
    for (size_t i = 0; GLOBAL_OPTIONS[i].name != NULL; i++) {
        fprintf(
            stderr,
            "  -%c, --%-*s %s\n",
            GLOBAL_OPTIONS[i].val,
            padding - (int)(lengthof("-x, --") - 1),
            GLOBAL_OPTIONS[i].name,
            GLOBAL_OPTIONS_DESC[i]
        );
    }

    fprintf(stderr, "\ncommands:\n");
    for (size_t i = 0; i < COMMANDS_LEN; i++)
        fprintf(
            stderr,
            "  %-*s %s\n",
            padding,
            COMMANDS[i].name,
            COMMANDS[i].desc
        );
}

int main(int argc, char **argv)
{
    int opt;
    const char *cmd_name;
    RuntimeOpts s;

    while ((opt = getopt_long(argc, argv, "+lh", GLOBAL_OPTIONS, NULL)) != -1) {
        switch (opt) {
            case 'l': s.enable_verbose_logging = true; break;
            case 'h': print_global_usage(); return EXIT_SUCCESS;
            default:  print_global_usage(); return EXIT_FAILURE;
        }
    }

    if (s.enable_verbose_logging)
        fprintf(stderr, "verbose mode enabled\n");

    if (optind >= argc) {
        print_global_usage();
        return EXIT_FAILURE;
    }

    cmd_name = argv[optind];

    for (size_t i = 0; i < COMMANDS_LEN; i++) {
        if (strcmp(COMMANDS[i].name, cmd_name) != 0) continue;

        if (s.enable_verbose_logging)
            fprintf(stderr, "cmd handler [%s]\n", COMMANDS[i].name);

        return COMMANDS[i].handler(&s, argc - optind, argv + optind);
    }

    fprintf(stderr, "scn: unknown command '%s'\n", cmd_name);
    print_global_usage();
    return EXIT_FAILURE;
}
