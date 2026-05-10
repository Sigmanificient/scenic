#ifndef SCENICOS_H
#define SCENICOS_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define SCENICOS_SCHEMA_VERSION 1

typedef struct {
    const char *const *data;
    size_t             len;
} strs;

typedef struct pkg pkg;

typedef struct {
    const pkg *const *data;
    size_t            len;
} pkg_refs;

typedef enum {
    BUILD_AUTOTOOLS = 1,
    BUILD_CMAKE     = 2,
    BUILD_MESON     = 3,
    BUILD_MAKE      = 4,
    BUILD_CARGO     = 5,
    BUILD_GO        = 6,
    BUILD_ZIG       = 7,
    BUILD_SHELL     = 99,
} build_system;

struct pkg {
    const char  *name;
    const char  *version;
    const char  *src;
    const char  *sha256;
    pkg_refs     deps;
    const char  *build_flags;
    build_system build_sys;
};

typedef struct {
    const char *name;
    bool        enabled;
    strs        after;
} service;

typedef struct {
    const service *data;
    size_t         len;
} services;

typedef struct {
    const char *name;
    const pkg  *shell;
    const char *home;
    strs        groups;
} user;

typedef struct {
    const user *data;
    size_t      len;
} users;

typedef struct {
    const char *bootloader;
    const pkg  *kernel;
    const char *kernel_params;
} boot_cfg;

typedef struct {
    uint32_t    schema_version;
    const char *hostname;
    boot_cfg    boot;
    pkg_refs    pkgs;
    services    services;
    users       users;
} system_cfg;

#define SYSTEM_CFG_INIT(...) { \
    .schema_version = SCENICOS_SCHEMA_VERSION, \
    __VA_ARGS__ \
}

#endif
