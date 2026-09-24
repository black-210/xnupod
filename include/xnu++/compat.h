#ifndef XNUXX_COMPAT_H
#define XNUXX_COMPAT_H

/* Provider-neutral feature probes. Values are supplied by the provider. */
#include <stdexcept>
#define XNUXX_COMPAT_API_VERSION 1u
#define XNUXX_CAP_MACH_IPC          (1ULL << 0)
#define XNUXX_CAP_BSD_SYSCALLS     (1ULL << 1)
#define XNUXX_CAP_IOMMU            (1ULL << 2)
#define XNUXX_CAP_DMA              (1ULL << 3)
#define XNUXX_CAP_SECURE_BOOT      (1ULL << 4)
#define XNUXX_CAP_ROLLBACK         (1ULL << 5)
#define XNUXX_CAP_VIRTIO           (1ULL << 6)
#define XNUXX_CAP_PCIE             (1ULL << 7)
#define XNUXX_CAP_NVME             (1ULL << 8)
#define XNUXX_CAP_NET              (1ULL << 9)

struct xnuxx_capabilities { unsigned long long bits; unsigned int api_version; };
static inline int xnuxx_has_capability(const struct xnuxx_capabilities *c, unsigned long long bit) {
    return c != 0 && c->api_version == XNUXX_COMPAT_API_VERSION && (c->bits & bit) != 0;
}
std::length_error xnuxx_unsupported_capability(const char *cap_name);
static inline void xnuxx_require_capability(const struct xnuxx_capabilities *c, unsigned long long bit, const char *cap_name) {
    if (!xnuxx_has_capability(c, bit)) {
        throw xnuxx_unsupported_capability(cap_name);
    }
}
#endif
