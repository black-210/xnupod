#ifndef XNUXX_RELIABILITY_H
#define XNUXX_RELIABILITY_H

#include <stddef.h>
#include <stdint.h>

#define XNUXX_RELIABILITY_API_VERSION 1u
#define XNUXX_PROVIDER_NAME_MAX 48u

enum xnuxx_provider_state {
    XNUXX_PROVIDER_UNKNOWN = 0,
    XNUXX_PROVIDER_READY = 1,
    XNUXX_PROVIDER_DEGRADED = 2,
    XNUXX_PROVIDER_QUARANTINED = 3,
    XNUXX_PROVIDER_STOPPED = 4
};

enum xnuxx_failover_class {
    XNUXX_FAILOVER_NEVER = 0,
    XNUXX_FAILOVER_SAFE_READONLY = 1,
    XNUXX_FAILOVER_SERVICE = 2
};

struct xnuxx_provider_health {
    uint32_t api_version;
    enum xnuxx_provider_state state;
    enum xnuxx_failover_class failover;
    uint64_t generation;
    uint64_t last_error;
    char provider[XNUXX_PROVIDER_NAME_MAX];
};

struct xnuxx_provider_ops {
    int (*health)(struct xnuxx_provider_health *out);
    int (*quiesce)(uint64_t deadline_ns);
    int (*resume)(void);
};

/* A provider may be replaced only after quiesce succeeds and policy permits it. */
int xnuxx_provider_register(const struct xnuxx_provider_ops *ops);
int xnuxx_provider_unregister(const struct xnuxx_provider_ops *ops);
int xnuxx_provider_health(struct xnuxx_provider_health *out);
int xnuxx_provider_failover(enum xnuxx_failover_class class, uint64_t deadline_ns);

#endif
