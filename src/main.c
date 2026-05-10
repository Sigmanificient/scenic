#define _GNU_SOURCE
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/nbos.h"
#include "activate.h"
#include "arena.h"
#include "error.h"
#include "realize.h"
#include "resolve.h"
#include "validate.h"

extern const system_cfg CFG;

static uint32_t next_generation_number(void) {
    return 1;
}

static int cmd_switch(void) {
    if (CFG.schema_version != NBOS_SCHEMA_VERSION) {
        fprintf(stderr,
                "nb: config schema version %u does not match nb %u\n",
                CFG.schema_version, NBOS_SCHEMA_VERSION);
        return 1;
    }

    arena *a = arena_create(1 << 20);
    if (a == nullptr) {
        fprintf(stderr, "nb: arena alloc failed\n");
        return 1;
    }

    fprintf(stderr, "nb: validating config\n");
    if (!validate(&CFG)) {
        arena_destroy(a);
        return 1;
    }

    fprintf(stderr, "nb: resolving %zu packages\n", CFG.pkgs.len);
    resolved_list resolved = {0};
    resolve_error rerr = resolve(a, &CFG, &resolved);
    if (rerr.kind != RESOLVE_OK) {
        resolve_error_print(&rerr);
        arena_destroy(a);
        return 1;
    }

    fprintf(stderr, "nb: realizing\n");
    realize_error realerr = realize(a, &CFG.pkgs, &resolved);
    if (realerr.kind != REALIZE_OK) {
        realize_error_print(&realerr);
        arena_destroy(a);
        return 1;
    }

    uint32_t gen = next_generation_number();
    fprintf(stderr, "nb: activating generation %u\n", gen);
    int aerr = activate(a, &CFG, &resolved, gen);
    if (aerr != 0) {
        fprintf(stderr, "nb: activation failed: %s\n", strerror(aerr));
        arena_destroy(a);
        return 1;
    }

    fprintf(stderr, "nb: switch complete (generation %u)\n", gen);
    arena_destroy(a);
    return 0;
}

static int cmd_validate(void) {
    if (CFG.schema_version != NBOS_SCHEMA_VERSION) {
        fprintf(stderr, "nb: schema version mismatch\n");
        return 1;
    }
    if (!validate(&CFG)) return 1;

    arena *a = arena_create(1 << 20);
    if (a == nullptr) return 1;

    resolved_list resolved = {0};
    resolve_error rerr = resolve(a, &CFG, &resolved);
    if (rerr.kind != RESOLVE_OK) {
        resolve_error_print(&rerr);
        arena_destroy(a);
        return 1;
    }

    fprintf(stderr, "nb: config is valid (%zu pkgs, %zu services, %zu users)\n",
            CFG.pkgs.len, CFG.services.len, CFG.users.len);
    arena_destroy(a);
    return 0;
}

static int cmd_rollback(void) {
    arena *a = arena_create(1 << 16);
    if (a == nullptr) return 1;
    int rc = activate_rollback(a);
    arena_destroy(a);
    return rc == 0 ? 0 : 1;
}

static void usage(void) {
    fprintf(stderr,
        "usage: nb <command>\n"
        "\n"
        "commands:\n"
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

    if (strcmp(cmd, "switch") == 0)   return cmd_switch();
    if (strcmp(cmd, "validate") == 0) return cmd_validate();
    if (strcmp(cmd, "rollback") == 0) return cmd_rollback();

    fprintf(stderr, "nb: unknown command '%s'\n", cmd);
    usage();
    return 1;
}
