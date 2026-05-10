#include "realize.h"

#include "build.h"

/**
 * realize() - Build every resolved package into the store.
 * @a: Arena for transient allocations.
 * @all_pkgs: Full package list (passed through to build_pkg()).
 * @resolved: Topologically-sorted resolved list. Builds happen in order.
 *
 * Stops at the first failure and returns its error.
 *
 * Return: REALIZE_OK_VAL on success, the failing package's error otherwise.
 */
realize_error realize(
    arena              *a,
    const pkg_refs     *all_pkgs,
    const resolved_list *resolved
) {
    for (size_t i = 0; i < resolved->len; i++) {
        realize_error err = build_pkg(
            a,
            resolved->data[i].def,
            all_pkgs,
            resolved->data,
            i
        );
        if (err.kind != REALIZE_OK) return err;
    }
    return REALIZE_OK_VAL;
}
