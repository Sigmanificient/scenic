#!/bin/sh
set -eu

mkdir -p build && cd build
 "$SCN_SRCDIR/configure" \
    --prefix="$SCN_PREFIX" \
    --disable-werror
make -j"$SCN_JOBS"
make install DESTDIR="$SCN_OUT"
