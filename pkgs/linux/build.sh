#!/bin/sh
set -eu

make -j"$SCM_JOBS" defconfig
make -j"$SCN_JOBS"
mkdir -p "$SCN_OUT/boot"
cp arch/x86/boot/bzImage "$SCN_OUT/boot/vmlinuz"
make INSTALL_MOD_PATH="$SCN_OUT" modules_install
