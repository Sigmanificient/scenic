#!/bin/sh
set -eu

zig build install \
    -Doptimize=ReleaseSafe \
    --prefix "$NB_OUT/usr"

install -Dm644 resources/oxwm.desktop -t "$NB_OUT/usr/share/xsessions"
install -Dm644 resources/oxwm.1       -t "$NB_OUT/usr/share/man/man1"
install -Dm644 templates/oxwm.lua     -t "$NB_OUT/usr/share/oxwm"
