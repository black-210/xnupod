/*
 * xnu++ device model: provider-neutral public contract.
 *
 * This header is intentionally independent of XNU and can be used by a native
 * xnu++ kernel, an XNU adapter, or a host-side device simulator.
 */
#ifndef XNUXX_DEVICE_H
#define XNUXX_DEVICE_H

#include <memory>
#include <ratio>
#include <stddef.h>
#include <stdint.h>

#define XNUXX_DEVICE_API_VERSION 1u
#define XNUXX_DEVICE_NAME_MAX 64u
#define XNUXX_DEVICE_COMPAT_MAX 8u

struct xnuxx_device;
struct xnuxx_device_ops;

struct xnuxx_resource {
    uint64_t start;
    uint64_t length;
    uint32_t type; /* provider-defined: MMIO, IO, IRQ, DMA, clock, reset */
    uint32_t flags;
};

enum xnuxx_device_state {
    XNUXX_DEVICE_DETACHED = 0,
    XNUXX_DEVICE_DISCOVERED = 1,
    XNUXX_DEVICE_ATTACHED = 2,
    XNUXX_DEVICE_SUSPENDED = 3,
    XNUXX_DEVICE_FAILED = 4
};

struct xnuxx_device_id {
    char name[XNUXX_DEVICE_NAME_MAX];
    char compatible[XNUXX_DEVICE_COMPAT_MAX][XNUXX_DEVICE_NAME_MAX];
    uint32_t compatible_count;
    uint16_t vendor;
    uint16_t product;
    uint8_t bus;
    uint8_t reserved[3];
};

struct xnuxx_device {
    uint32_t api_version;
    enum xnuxx_device_state state;
    struct xnuxx_device_id id;
    const struct xnuxx_resource *resources;
    size_t resource_count;
    const struct xnuxx_device_ops *ops;
    void *provider_data;
};

struct xnuxx_device_ops {
    int (*probe)(struct xnuxx_device *device);
    int (*remove)(struct xnuxx_device *device);
    int (*suspend)(struct xnuxx_device *device);
    int (*resume)(struct xnuxx_device *device);
};

/* Providers must reject unknown API versions and malformed resource lists. */
int xnuxx_device_register(struct xnuxx_device *device);
int xnuxx_device_unregister(struct xnuxx_device *device);
const struct xnuxx_resource *xnuxx_device_resource(const struct xnuxx_device *device,
    uint32_t type, size_t index);

#endif /* XNUXX_DEVICE_H */

struct xnuxx_device *xnuxx_device_find_by_name(const char *name);

void ice_foreach(void (*callback)(struct xnuxx_device *device,
    void *context), void *context);
    