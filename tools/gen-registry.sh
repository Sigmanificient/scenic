#!/bin/sh
# Walks pkgs/ and emits all.h (declarations) and all.c (registry array).
# Run via `make registry`.
#
# A recipe is a directory pkgs/<cat>/<name>/ containing <name>.c and <name>.h.
# The .h must declare `extern const pkg pkgs_<name>;` and the .c must define it.

set -eu

PKGS_DIR="${PKGS_DIR:-pkgs}"
MODE="${1:-h}"

recipes=$(find "$PKGS_DIR" -mindepth 2 -maxdepth 2 -name '*.h' \
            ! -name 'all.h' | sort)

if [ "$MODE" = "h" ] || [ "$MODE" = "--h" ]; then
    printf '/* AUTO-GENERATED. Do not edit. */\n'
    printf '#ifndef PKGS_ALL_H\n'
    printf '#define PKGS_ALL_H\n\n'
    printf '#include "../include/nbos.h"\n\n'

    for h in $recipes; do
        name=$(basename "$h" .h)
        printf 'extern const pkg pkgs_%s;\n' "$name"
    done

    printf '\n'
    printf 'extern const pkg *const PKGS_REGISTRY[];\n'
    printf 'extern const size_t   PKGS_REGISTRY_LEN;\n\n'
    printf '#endif\n'
elif [ "$MODE" = "c" ] || [ "$MODE" = "--c" ]; then
    printf '/* AUTO-GENERATED. Do not edit. */\n'
    printf '#include "all.h"\n\n'
    printf 'const pkg *const PKGS_REGISTRY[] = {\n'

    for h in $recipes; do
        name=$(basename "$h" .h)
        printf '    &pkgs_%s,\n' "$name"
    done

    printf '};\n\n'
    printf 'const size_t PKGS_REGISTRY_LEN = sizeof(PKGS_REGISTRY) / sizeof(PKGS_REGISTRY[0]);\n'
else
    printf 'usage: %s [--h|--c]\n' "$0" >&2
    exit 1
fi
