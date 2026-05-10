#ifndef SCENIC_RUN_H
#define SCENIC_RUN_H

#include "error.h"

run_error run(
    char *const  argv[],
    char *const  envp[],
    const char  *cwd,
    const char  *log_path);

#endif
