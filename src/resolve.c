#include "resolve.h"

#include <stdint.h>
#include <stdalign.h>
#include <string.h>

#include "hash.h"
#include "store.h"

typedef enum : uint8_t {
    UNVISITED = 0,
    VISITING,
    VISITED,
} visit_state;

static size_t find_pkg(const pkg_refs *pkgs, const pkg *needle) {
    for (size_t i = 0; i < pkgs->len; i++) {
        if (pkgs->data[i] == needle) return i;
    }
    return SIZE_MAX;
}

static resolve_error visit(
    arena              *a,
    const pkg_refs     *pkgs,
    size_t              idx,
    visit_state        *state,
    resolved           *out)
{
    switch (state[idx]) {
        case VISITED:  return RESOLVE_OK_VAL;
        case VISITING: return (resolve_error){
            .kind = RESOLVE_E_CYCLE,
            .pkg_name = pkgs->data[idx]->name,
        };
        case UNVISITED: break;
    }

    state[idx] = VISITING;
    const pkg *p = pkgs->data[idx];

    for (size_t i = 0; i < p->deps.len; i++) {
        const pkg *dep = p->deps.data[i];
        size_t dep_idx = find_pkg(pkgs, dep);
        if (dep_idx == SIZE_MAX) {
            return (resolve_error){
                .kind = RESOLVE_E_MISSING_DEP,
                .pkg_name = p->name,
                .dep_name = dep->name,
            };
        }

        resolve_error err = visit(a, pkgs, dep_idx, state, out);
        if (err.kind != RESOLVE_OK) return err;
    }

    out[idx].def = p;
    out[idx].store_path = store_path_compute(a, p, pkgs, out);

    state[idx] = VISITED;
    return RESOLVE_OK_VAL;
}

/**
 * resolve() - Topologically sort packages and compute their store paths.
 * @a: Arena holding the resulting visit-state and resolved arrays.
 * @cfg: System configuration providing the package list.
 * @out: Filled with topologically-sorted &resolved entries on success.
 *
 * Rejects duplicate package names up front, then performs a DFS that
 * detects cycles and missing deps. Each entry's store path depends on
 * its deps' store paths, so children must be resolved first.
 *
 * Return: RESOLVE_OK_VAL on success, a tagged error otherwise.
 */
resolve_error resolve(
    arena              *a,
    const system_cfg   *cfg,
    resolved_list      *out)
{
    for (size_t i = 0; i < cfg->pkgs.len; i++) {
        for (size_t j = i + 1; j < cfg->pkgs.len; j++) {
            if (strcmp(cfg->pkgs.data[i]->name, cfg->pkgs.data[j]->name) == 0) {
                return (resolve_error){
                    .kind = RESOLVE_E_DUPLICATE_NAME,
                    .pkg_name = cfg->pkgs.data[i]->name,
                };
            }
        }
    }

    visit_state *state = arena_alloc(a, cfg->pkgs.len * sizeof(visit_state),
                                     alignof(visit_state));
    resolved    *res   = arena_alloc(a, cfg->pkgs.len * sizeof(resolved),
                                     alignof(resolved));
    if (state == nullptr || res == nullptr) {
        return (resolve_error){ .kind = RESOLVE_E_MISSING_DEP };
    }

    for (size_t i = 0; i < cfg->pkgs.len; i++) {
        resolve_error err = visit(a, &cfg->pkgs, i, state, res);
        if (err.kind != RESOLVE_OK) return err;
    }

    out->data = res;
    out->len  = cfg->pkgs.len;
    return RESOLVE_OK_VAL;
}
