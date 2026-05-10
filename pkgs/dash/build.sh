#!/bin/sh
set -eu

./configure --prefix="$SCN_PREFIX"
make -j"$SCN_JOBS"
make install DESTDIR="$SCN_OUT"
