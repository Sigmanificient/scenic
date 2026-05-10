#!/bin/sh
set -eu

cmake -B build -S . \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX="$SCN_OUT/usr"
cmake --build build --parallel "$SCN_JOBS"
cmake --install build
