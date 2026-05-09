#!/bin/sh
set -eu

mkdir -p build && cd build
"$NB_SRCDIR/configure" \
    --prefix="$NB_PREFIX" \
    --disable-werror
make -j"$NB_JOBS"
make install DESTDIR="$NB_OUT"
