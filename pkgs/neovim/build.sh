#!/bin/sh
set -eu

cmake -B build -S . \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX="$NB_OUT/usr"
cmake --build build --parallel "$NB_JOBS"
cmake --install build
