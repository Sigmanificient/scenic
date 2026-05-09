# nb

A from-source declarative Linux distribution written in disciplined C23.

## Build

    make registry
    make
    sudo ./nb validate
    sudo ./nb switch

## design choices (subject to change)

- **No daemon.** Single-user model. `nb switch` runs as root, does its
  work, exits. No persistent process, no IPC, no socket protocol.
- **No custom config DSL for v0.1.** The user's config is plain C against
  the schema. A translator to a friendlier syntax can be added later
  without changing the core.
- **Content-addressed store.** `/nb/store/<hash>-<name>-<version>/`.
  The hash includes the pkg's deps' store paths transitively, so any
  change in any dep reshuffles every dependent.
- **Generations are immutable; rollback is symlink swap.** The previous
  generation always remains bootable until garbage-collected.
