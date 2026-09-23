#ifndef XNUXX_BUS_H
#define XNUXX_BUS_H

#include <stddef.h>
#include <stdint.h>
#include "device.h"

enum xnuxx_bus_type { XNUXX_BUS_VIRTIO = 1, XNUXX_BUS_PCIE = 2, XNUXX_BUS_USB = 3, XNUXX_BUS_I2C = 4, XNUXX_BUS_SPI = 5 };
struct xnuxx_bus;
struct xnuxx_bus_ops {
    int (*enumerate)(struct xnuxx_bus *, struct xnuxx_device *, size_t, size_t *);
    int (*reset)(struct xnuxx_bus *);
};
struct xnuxx_bus { uint32_t api_version; enum xnuxx_bus_type type; const struct xnuxx_bus_ops *ops; void *provider_data; };
int xnuxx_bus_register(struct xnuxx_bus *);
int xnuxx_bus_unregister(struct xnuxx_bus *);
#endif
