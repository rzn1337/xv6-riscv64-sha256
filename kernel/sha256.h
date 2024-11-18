#ifndef SHA256_H
#define SHA256_H

#include "types.h"
#include <stddef.h>
#include <stdint.h>



// SHA-256 context structure
typedef struct {
    uint8_t data[64];
    uint32_t datalen;
    uint64_t bitlen;
    uint32_t state[8];
} SHA256_CTX;

// Function declarations
void sha256_init(SHA256_CTX *ctx);
void sha256_update(SHA256_CTX *ctx, const uint8_t data[], size_t len);
void sha256_final(SHA256_CTX *ctx, uint8_t hash[]);

#endif