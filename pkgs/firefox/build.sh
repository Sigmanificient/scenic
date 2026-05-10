#!/bin/sh
set -eu

./mach build
./mach package
mkdir -p "$NB_OUT/usr"
tar -xf obj-*/dist/firefox-*.tar.bz2 -C "$NB_OUT/usr"
