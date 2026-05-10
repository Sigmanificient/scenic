#!/bin/sh
set -eu

make -j"$SCN_JOBS" linux
make install INSTALL_TOP="$SCN_OUT/usr"
