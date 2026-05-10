#!/bin/sh
set -eu

./configure --prefix="$NB_PREFIX"
make -j"$NB_JOBS"
make install DESTDIR="$NB_OUT"
