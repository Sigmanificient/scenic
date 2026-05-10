#!/bin/sh
set -eu

make -j"$NB_JOBS" linux
make install INSTALL_TOP="$NB_OUT/usr"
