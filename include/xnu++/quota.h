#ifndef XNUXX_QUOTA_H
#define XNUXX_QUOTA_H

#include <stdint.h>

#define XNUXX_QUOTA_API_VERSION 1u

enum xnuxx_quota_kind { XNUXX_QUOTA_CPU = 1, XNUXX_QUOTA_MEMORY = 2, XNUXX_QUOTA_IO = 3, XNUXX_QUOTA_HANDLES = 4 };
struct xnuxx_quota { uint32_t api_version; enum xnuxx_quota_kind kind; uint64_t soft_limit; uint64_t hard_limit; uint64_t usage; };

int xnuxx_quota_set(uint64_t subject, const struct xnuxx_quota *);
int xnuxx_quota_charge(uint64_t subject, enum xnuxx_quota_kind, uint64_t amount);
int xnuxx_quota_release(uint64_t subject, enum xnuxx_quota_kind, uint64_t amount);
int xnuxx_quota_read(uint64_t subject, enum xnuxx_quota_kind, struct xnuxx_quota *);

#endif
