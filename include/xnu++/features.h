#ifndef XNUXX_FEATURES_H
#define XNUXX_FEATURES_H

#include <stdint.h>

#define XNUXX_FEATURES_API_VERSION 1u
#define XNUXX_FEATURE_CHECKPOINTS        (1ULL << 0)
#define XNUXX_FEATURE_TRANSACTIONAL_UPDATE (1ULL << 1)
#define XNUXX_FEATURE_RESOURCE_QUOTAS    (1ULL << 2)
#define XNUXX_FEATURE_SERVICE_ISOLATION  (1ULL << 3)
#define XNUXX_FEATURE_STRUCTURED_CRASH   (1ULL << 4)
#define XNUXX_FEATURE_DEVICE_SIMULATOR   (1ULL << 5)
#define XNUXX_FEATURE_LIVE_DIAGNOSTICS   (1ULL << 6)
#define XNUXX_FEATURE_DETERMINISTIC_TEST (1ULL << 7)

struct xnuxx_feature_set {
    uint32_t api_version;
    uint32_t reserved;
    uint64_t enabled;
    uint64_t required;
};

static inline int xnuxx_features_satisfy(const struct xnuxx_feature_set *set) {
    return set != 0 && set->api_version == XNUXX_FEATURES_API_VERSION &&
        (set->enabled & set->required) == set->required;
}

#endif
