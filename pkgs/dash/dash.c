#include "dash.h"
#include "../glibc/glibc.h"

static const pkg *const dash_deps[] = { &pkgs_glibc };

const pkg pkgs_dash = {
    .name        = "dash",
    .version     = "0.5.12",
    .src         = "http://gondor.apana.org.au/~herbert/dash/files/dash-0.5.12.tar.gz",
    .sha256      = "0000000000000000000000000000000000000000000000000000000000000000",
    .deps        = { .data = dash_deps, .len = sizeof(dash_deps) / sizeof(dash_deps[0]) },
    .build_flags = "",
    .build_sys   = BUILD_AUTOTOOLS,
};
