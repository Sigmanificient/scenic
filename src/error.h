#ifndef NB_ERROR_H
#define NB_ERROR_H

#include <stddef.h>
#include <stdint.h>

typedef enum : uint8_t {
    RESOLVE_OK = 0,
    RESOLVE_E_CYCLE,
    RESOLVE_E_MISSING_DEP,
    RESOLVE_E_DUPLICATE_NAME,
} resolve_error_kind;

typedef struct {
    resolve_error_kind kind;
    const char        *pkg_name;
    const char        *dep_name;
} resolve_error;

#define RESOLVE_OK_VAL ((resolve_error){ .kind = RESOLVE_OK })

typedef enum : uint8_t {
    FETCH_OK = 0,
    FETCH_E_NETWORK,
    FETCH_E_HASH_MISMATCH,
    FETCH_E_IO,
} fetch_error_kind;

typedef struct {
    fetch_error_kind kind;
    const char      *url;
    const char      *expected_sha;
    const char      *actual_sha;
    int              errno_val;
} fetch_error;

#define FETCH_OK_VAL ((fetch_error){ .kind = FETCH_OK })

typedef enum : uint8_t {
    RUN_OK = 0,
    RUN_E_SPAWN,
    RUN_E_NONZERO,
    RUN_E_IO,
} run_error_kind;

typedef struct {
    run_error_kind kind;
    int            exit_code;
    const char    *log_path;
    int            errno_val;
} run_error;

#define RUN_OK_VAL ((run_error){ .kind = RUN_OK })

typedef enum : uint8_t {
    REALIZE_OK = 0,
    REALIZE_E_FETCH,
    REALIZE_E_BUILD,
    REALIZE_E_SANDBOX,
    REALIZE_E_STORE,
} realize_error_kind;

typedef struct {
    realize_error_kind kind;
    const char        *pkg_name;
    union {
        fetch_error fetch;
        run_error   run;
        int         errno_val;
    };
} realize_error;

#define REALIZE_OK_VAL ((realize_error){ .kind = REALIZE_OK })

void resolve_error_print(const resolve_error *e);
void fetch_error_print(const fetch_error *e);
void run_error_print(const run_error *e);
void realize_error_print(const realize_error *e);

#endif
