#!/bin/sh

set -eu

PKGS_DIR="${PKGS_DIR:-pkgs}"

OUT_H="$PKGS_DIR/all.h"
OUT_C="$PKGS_DIR/all.c"

recipes=$(find "$PKGS_DIR" -mindepth 2 -name '*.h' | sort)

{
    printf '/* AUTO-GENERATED. Do not edit. */\n'
    printf '#ifndef PKGS_ALL_H\n'
    printf '#define PKGS_ALL_H\n\n'

    printf '#include "scenicos.h"\n\n'

    for h in $recipes; do
        name=$(basename "$h" .h)
        printf 'extern const pkg pkgs_%s;\n' "$name"
    done

    printf '\n'
    printf 'extern const pkg *const PKGS_REGISTRY[];\n'
    printf 'extern const size_t PKGS_REGISTRY_LEN;\n\n'

    printf '#endif\n'
} > "$OUT_H"

{
    printf '/* AUTO-GENERATED. Do not edit. */\n'
    printf '#include "all.h"\n\n'

    printf 'const pkg *const PKGS_REGISTRY[] = {\n'

    for h in $recipes; do
        name=$(basename "$h" .h)
        printf '    &pkgs_%s,\n' "$name"
    done

    printf '};\n\n'

    printf 'const size_t PKGS_REGISTRY_LEN = '
    printf 'sizeof(PKGS_REGISTRY) / sizeof(PKGS_REGISTRY[0]);\n'
} > "$OUT_C"
