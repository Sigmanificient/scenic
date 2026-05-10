#!/bin/sh
set -eu

make -j"$NB_JOBS" defconfig
make -j"$NB_JOBS"
mkdir -p "$NB_OUT/boot"
cp arch/x86/boot/bzImage "$NB_OUT/boot/vmlinuz"
make INSTALL_MOD_PATH="$NB_OUT" modules_install
