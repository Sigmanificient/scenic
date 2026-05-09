#include "validate.h"

#include <stdio.h>
#include <string.h>

static bool pkg_in_list(const pkg *p, const pkg_refs *list) {
    for (size_t i = 0; i < list->len; i++) {
        if (list->data[i] == p) return true;
    }
    return false;
}

static bool is_known_bootloader(const char *name) {
    return strcmp(name, "limine")   == 0
        || strcmp(name, "grub")     == 0
        || strcmp(name, "syslinux") == 0;
}

/**
 * validate() - Run static checks on the system config.
 * @cfg: Configuration to inspect.
 *
 * Covers what resolve() does not: hostname presence, bootloader
 * recognition, kernel/shell pointers belong to the package list, and
 * uniqueness of user and service names. Errors are printed to stderr.
 *
 * Return: true if every check passes.
 */
bool validate(const system_cfg *cfg) {
    bool ok = true;

    if (cfg->hostname == nullptr || cfg->hostname[0] == '\0') {
        fprintf(stderr, "nb: hostname is empty\n");
        ok = false;
    }

    if (!is_known_bootloader(cfg->boot.bootloader)) {
        fprintf(stderr, "nb: unknown bootloader '%s'\n", cfg->boot.bootloader);
        ok = false;
    }

    if (cfg->boot.kernel == nullptr) {
        fprintf(stderr, "nb: boot.kernel is null\n");
        ok = false;
    } else if (!pkg_in_list(cfg->boot.kernel, &cfg->pkgs)) {
        fprintf(stderr, "nb: boot.kernel '%s' is not in the package list\n",
                cfg->boot.kernel->name);
        ok = false;
    }

    for (size_t i = 0; i < cfg->users.len; i++) {
        const user *u = &cfg->users.data[i];

        if (u->name == nullptr || u->name[0] == '\0') {
            fprintf(stderr, "nb: user[%zu] has empty name\n", i);
            ok = false;
            continue;
        }

        if (u->shell == nullptr) {
            fprintf(stderr, "nb: user '%s' has null shell\n", u->name);
            ok = false;
        } else if (!pkg_in_list(u->shell, &cfg->pkgs)) {
            fprintf(stderr, "nb: user '%s' shell '%s' is not in the package list\n",
                    u->name, u->shell->name);
            ok = false;
        }

        for (size_t j = i + 1; j < cfg->users.len; j++) {
            if (strcmp(u->name, cfg->users.data[j].name) == 0) {
                fprintf(stderr, "nb: duplicate user name '%s'\n", u->name);
                ok = false;
            }
        }
    }

    for (size_t i = 0; i < cfg->services.len; i++) {
        const service *s = &cfg->services.data[i];

        if (s->name == nullptr || s->name[0] == '\0') {
            fprintf(stderr, "nb: service[%zu] has empty name\n", i);
            ok = false;
            continue;
        }

        for (size_t j = i + 1; j < cfg->services.len; j++) {
            if (strcmp(s->name, cfg->services.data[j].name) == 0) {
                fprintf(stderr, "nb: duplicate service name '%s'\n", s->name);
                ok = false;
            }
        }
    }

    return ok;
}
