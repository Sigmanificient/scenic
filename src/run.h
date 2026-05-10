#ifndef SCENIC_RUN_H
#define SCENIC_RUN_H

#include "error.h"

[[nodiscard]] run_error run(
    const char  *cmd,
    char *const  argv[],
    char *const  envp[],
    const char  *cwd,
    const char  *log_path);

#endif
