#ifndef SCENIC_SANDBOX_H
#define SCENIC_SANDBOX_H

#include "../include/scenicos.h"
#include "resolve.h"

int sandbox_setup(
    const char     *sandbox_root,
    const pkg_refs *deps,
    const resolved *resolved_deps,
    const char     *src_dir);

void sandbox_teardown(const char *sandbox_root);

#endif
