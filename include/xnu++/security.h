#ifndef XNUXX_SECURITY_H
#define XNUXX_SECURITY_H

#include <stddef.h>
#include <stdint.h>

#define XNUXX_SECURITY_API_VERSION 1u
#define XNUXX_HASH_SHA256 1u
#define XNUXX_POLICY_DENY 0u
#define XNUXX_POLICY_ALLOW 1u

enum xnuxx_security_event {
    XNUXX_SEC_BOOT_VERIFY = 1,
    XNUXX_SEC_DRIVER_LOAD = 2,
    XNUXX_SEC_RESOURCE_ACCESS = 3,
    XNUXX_SEC_UPDATE_VERIFY = 4,
    XNUXX_SEC_ROLLBACK = 5
};

struct xnuxx_measurement {
    uint32_t algorithm;
    uint32_t length;
    uint8_t digest[32];
};

struct xnuxx_security_context {
    uint32_t api_version;
    uint32_t policy_flags;
    uint64_t subject_id;
    uint64_t audit_sequence;
};

/* Implementations must fail closed on malformed inputs or unknown policies. */
int xnuxx_security_authorize(const struct xnuxx_security_context *,
    enum xnuxx_security_event, const void *, size_t);
int xnuxx_security_measure(const void *, size_t, struct xnuxx_measurement *);
int xnuxx_security_verify_measurement(const void *, size_t,
    const struct xnuxx_measurement *);

#endif
