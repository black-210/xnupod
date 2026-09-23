#ifndef XNUXX_UPDATE_H
#define XNUXX_UPDATE_H

#include <stddef.h>
#include <stdint.h>

#define XNUXX_UPDATE_API_VERSION 1u
#define XNUXX_UPDATE_ID_MAX 64u

enum xnuxx_update_state {
    XNUXX_UPDATE_STAGED = 1,
    XNUXX_UPDATE_VERIFIED = 2,
    XNUXX_UPDATE_COMMITTED = 3,
    XNUXX_UPDATE_ROLLED_BACK = 4
};

struct xnuxx_update_manifest {
    uint32_t api_version;
    enum xnuxx_update_state state;
    uint64_t generation;
    uint64_t minimum_generation;
    uint8_t digest[32];
    char id[XNUXX_UPDATE_ID_MAX];
};

int xnuxx_update_stage(const void *, size_t, struct xnuxx_update_manifest *);
int xnuxx_update_verify(const struct xnuxx_update_manifest *);
int xnuxx_update_commit(const struct xnuxx_update_manifest *);
int xnuxx_update_rollback(uint64_t generation);

#endif
