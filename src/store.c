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

#include "store.h"

#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "hash.h"

static const char b32_alphabet[] = "0123456789abcdfghijklmnpqrsvwxyz";

static void base32_encode(const uint8_t *in, size_t len, char *out) {
    size_t bits = 0;
    uint32_t buf = 0;
    size_t out_idx = 0;

    for (size_t i = 0; i < len; i++) {
        buf = (buf << 8) | in[i];
        bits += 8;
        while (bits >= 5) {
            bits -= 5;
            out[out_idx++] = b32_alphabet[(buf >> bits) & 0x1f];
        }
    }
    if (bits > 0) {
        out[out_idx++] = b32_alphabet[(buf << (5 - bits)) & 0x1f];
    }
    out[out_idx] = '\0';
}

/**
 * store_path_compute() - Compute the content-addressed store path for @p.
 * @a: Arena that owns the returned string.
 * @p: Package whose inputs determine the path.
 * @resolved_pkgs: Resolved entries already topo-sorted; deps must already
 *                 have @store_path populated.
 * @n_resolved: Number of valid entries in @resolved_pkgs.
 *
 * Hashes a length-prefixed canonical encoding of the package inputs
 * (name, version, src URL, source sha256, build flags, build system,
 * resolved dep paths). The first 20 bytes of the digest are base32
 * encoded; identical inputs always produce the same path.
 *
 * Return: "/<hash>-<name>-<version>" owned by @a, or NULL if a
 * dep's store path is not yet resolved.
 */
store_path store_path_compute(
    arena              *a,
    const pkg          *p,
    const resolved     *resolved_pkgs,
    size_t              n_resolved)
{
    sha256_ctx ctx;
    sha256_init(&ctx);

    #define HASH_FIELD(s) do { \
        if ((s) != NULL) { \
            size_t l = strlen(s); \
            sha256_update(&ctx, &l, sizeof(l)); \
            sha256_update(&ctx, (s), l); \
            sha256_update(&ctx, "\0", 1); \
        } \
    } while (0)

    HASH_FIELD(p->name);
    HASH_FIELD(p->version);
    HASH_FIELD(p->src);
    HASH_FIELD(p->sha256);
    HASH_FIELD(p->build_flags);

    uint8_t bs = (uint8_t)p->build_sys;
    sha256_update(&ctx, &bs, 1);
    sha256_update(&ctx, "\0", 1);

    for (size_t i = 0; i < p->deps.len; i++) {
        const pkg *target = p->deps.data[i];
        const char *dep_path = NULL;
        for (size_t j = 0; j < n_resolved; j++) {
            if (resolved_pkgs[j].def == target) {
                dep_path = resolved_pkgs[j].out;
                break;
            }
        }
        if (dep_path == NULL) return NULL;
        HASH_FIELD(dep_path);
    }

    #undef HASH_FIELD

    uint8_t digest[32];
    sha256_final(&ctx, digest);

    char b32[33];
    base32_encode(digest, 20, b32);

    char *path = arena_sprintf(a, "%s/%s-%s-%s",
                         SCN_STORE_ROOT, b32, p->name, p->version);

    assert(path != NULL);
    return UNQUALIFY(path);
}

/**
 * store_path_exists() - Check whether @store_path is a populated store entry.
 * @target: Path to test.
 *
 * Return: true if @store_path is an existing directory.
 */
bool store_path_exists(store_path target) {
    struct stat st;
    if (stat(QUALIFY(target), &st) < 0) return false;
    return S_ISDIR(st.st_mode);
}

/**
 * store_install() - Atomically move a build output into the store.
 * @temp_path: Source directory produced by the build.
 * @target: Destination inside the store.
 *
 * Return: 0 on success, errno value on failure.
 */
int store_install(const char *temp_path, store_path target) {
    if (rename(temp_path, QUALIFY(target)) < 0) {
        return errno;
    }
    return 0;
}

/**
 * store_remove() - Recursively remove a store entry.
 * @target: Path to remove.
 *
 * Return: 0 on success, errno value on failure.
 */
int store_remove(store_path target) {
    char cmd[4096];
    int n = snprintf(cmd, sizeof(cmd), "rm -rf '%s'", QUALIFY(target));
    if (n < 0 || (size_t)n >= sizeof(cmd)) return ENAMETOOLONG;
    int rc = system(cmd);
    return rc == 0 ? 0 : EIO;
}
