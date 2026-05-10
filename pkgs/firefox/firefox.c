#include "firefox.h"
#include "../glibc/glibc.h"

static const pkg *const firefox_deps[] = { &pkgs_glibc };

const pkg pkgs_firefox = {
    .name        = "firefox",
    .version     = "133.0",
    .src         = "https://archive.mozilla.org/pub/firefox/releases/133.0/source/firefox-133.0.source.tar.xz",
    .sha256      = "0000000000000000000000000000000000000000000000000000000000000000",
    .deps        = { .data = firefox_deps, .len = sizeof(firefox_deps) / sizeof(firefox_deps[0]) },
    .build_flags = "",
    .build_sys   = BUILD_SHELL,
};
