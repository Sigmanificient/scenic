/* /etc/nb/config.c */
#include "../include/nbos.h"
#include "../pkgs/core/glibc/glibc.h"
#include "../pkgs/core/linux/linux.h"
#include "../pkgs/editor/neovim/neovim.h"
#include "../pkgs/wm/oxwm/oxwm.h"
#include "../pkgs/shell/dash/dash.h"
#include "../pkgs/browser/firefox/firefox.h"

static const pkg *const my_pkgs[] = {
    &pkgs_glibc,
    &pkgs_linux,
    &pkgs_neovim,
    &pkgs_oxwm,
    &pkgs_firefox,
};

static const char *const sshd_after[]  = { "network" };
static const service my_services[] = {
    { .name = "sshd", .enabled = true,
      .after = { .data = sshd_after, .len = 1 } },
};

static const char *const tony_groups[] = { "wheel", "video", "audio" };
static const user my_users[] = {
    { .name   = "tony",
      .shell  = &pkgs_dash,
      .home   = "/home/tony",
      .groups = { .data = tony_groups, .len = 3 } },
};

const system_cfg CFG = SYSTEM_CFG_INIT(
    .hostname = "nbos-btw",
    .boot = {
        .bootloader    = "limine",
        .kernel        = &pkgs_linux,
        .kernel_params = "quiet loglevel=3",
    },
    .pkgs     = { .data = my_pkgs,     .len = 5 },
    .services = { .data = my_services, .len = 1 },
    .users    = { .data = my_users,    .len = 1 },
);
