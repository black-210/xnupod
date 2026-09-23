#ifndef XNUXX_ISOLATION_H
#define XNUXX_ISOLATION_H

#include <stdint.h>

#define XNUXX_ISOLATION_API_VERSION 1u
#define XNUXX_ISOLATE_NO_RAW_IO (1u << 0)
#define XNUXX_ISOLATE_NO_PRIVILEGE_ESCALATION (1u << 1)
#define XNUXX_ISOLATE_NO_UNDECLARED_DEVICES (1u << 2)
#define XNUXX_ISOLATE_AUDIT_FAILURES (1u << 3)

struct xnuxx_isolation_policy { uint32_t api_version; uint32_t flags; uint64_t memory_limit; uint64_t cpu_limit; };
int xnuxx_isolation_create(uint64_t subject, const struct xnuxx_isolation_policy *);
int xnuxx_isolation_destroy(uint64_t subject);
int xnuxx_isolation_attach_device(uint64_t subject, uint64_t device_id);

#endif
