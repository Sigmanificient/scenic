#ifndef SCENIC_HASH_H
#define SCENIC_HASH_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct {
    uint32_t state[8];
    uint64_t bitcount;
    uint8_t  buffer[64];
    size_t   buffer_used;
} sha256_ctx;

void sha256_init  (sha256_ctx *ctx);
void sha256_update(sha256_ctx *ctx, const void *data, size_t len);
void sha256_final (sha256_ctx *ctx, uint8_t out[32]);

void sha256_hash(const void *data, size_t len, uint8_t out[32]);
void sha256_hex(const uint8_t digest[32], char out[65]);

bool sha256_verify_file(
    const char *path,
    const char *expected_hex,
    char        actual_hex[65]);

#endif
