# scenic

A from-source declarative Linux distribution written in disciplined C99.

## Build

    make registry
    make
    sudo ./scn validate
    sudo ./scn switch

## design choices (subject to change)

- **no daemon.** Single-user model. `scn switch` runs as root, does its
  work, exits. No persistent process, no IPC, no socket protocol.
- **no custom config DSL for v0.1.** The user's config is plain C against
  the schema. A translator to a friendlier syntax can be added later
  without changing the core.
- **content-addressed store.** `/scn/store/<hash>-<name>-<version>/`.
  The hash includes the pkg's deps' store paths transitively, so any
  change in any dep reshuffles every dependent.
- **generations are immutable; rollback is symlink swap.** The previous
  generation always remains bootable until garbage-collected.
