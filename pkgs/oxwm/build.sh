#!/bin/sh
set -eu

zig build install \
    -Doptimize=ReleaseSafe \
    --prefix "$SCN_OUT/usr"

install -Dm644 resources/oxwm.desktop -t "$SCN_OUT/usr/share/xsessions"
install -Dm644 resources/oxwm.1       -t "$SCN_OUT/usr/share/man/man1"
install -Dm644 templates/oxwm.lua     -t "$SCN_OUT/usr/share/oxwm"
