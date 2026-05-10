#define _GNU_SOURCE

#include <errno.h>

#include "activate.h"

/**
 * activate() - Build and activate a generation.
 * @a: Arena for transient allocations.
 * @cfg: System configuration to activate.
 * @resolved: Topologically-sorted resolved package list.
 * @generation: Generation number to assign.
 *
 * Symlinks resolved store paths into /scn/system/<gen>/, updates the
 * bootloader, atomically swaps /scn/system/current, and runs service
 * activations. On failure at any step, the previous generation
 * remains active.
 *
 * Return: 0 on success, errno value on failure.
 */
int activate(
    arena               *a,
    const system_cfg    *cfg,
    const resolved_list *resolved,
    uint32_t             generation)
{
    (void)a;
    (void)cfg;
    (void)resolved;
    (void)generation;
    return ENOSYS;
}

/**
 * activate_rollback() - Activate the previous generation.
 * @a: Arena for transient allocations.
 *
 * Return: 0 on success, errno value on failure.
 */
int activate_rollback(arena *a) {
    (void)a;
    return ENOSYS;
}
