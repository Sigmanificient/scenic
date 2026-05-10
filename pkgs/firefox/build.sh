#!/bin/sh
set -eu

./mach build
./mach package
mkdir -p "$SCN_OUT/usr"
tar -xf obj-*/dist/firefox-*.tar.bz2 -C "$SCN_OUT/usr"
