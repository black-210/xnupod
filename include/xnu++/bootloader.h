#ifndef XNUXX_BOOTLOADER_H
#define XNUXX_BOOTLOADER_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define XNUXX_BOOTLOADER_API_VERSION 1u
#define XNUXX_BOOTLOADER_MAGIC 0x584E5542u /* "XNUB" */
#define XNUXX_BOOTLOADER_MAX_IMAGE_SIZE (1024u * 1024u * 1024u)

#define XNUXX_BOOT_REQUIRE_SIGNATURE (1u << 0)
#define XNUXX_BOOT_REQUIRE_MEASUREMENT (1u << 1)
#define XNUXX_BOOT_REQUIRE_ROLLBACK_PROTECTION (1u << 2)
#define XNUXX_BOOT_REQUIRE_IOMMU (1u << 3)
#define XNUXX_BOOT_REQUIRE_LAYER_CAPABILITIES (1u << 4)
#define XNUXX_BOOT_STRICT (1u << 31)

#define XNUXX_BOOT_CAP_KERNEL_PROVIDER (1ULL << 0)
#define XNUXX_BOOT_CAP_SECURITY_LAYER (1ULL << 1)
#define XNUXX_BOOT_CAP_UPDATE_LAYER (1ULL << 2)
#define XNUXX_BOOT_CAP_DEVICE_LAYER (1ULL << 3)
#define XNUXX_BOOT_CAP_RECOVERY (1ULL << 4)

struct xnuxx_boot_image {
    uint32_t magic;
    uint16_t version;
    uint16_t header_size;
    uint64_t generation;
    uint64_t required_capabilities;
    const void *payload;
    size_t payload_size;
    const void *signature;
    size_t signature_size;
    const uint8_t *expected_measurement; /* SHA-256, 32 bytes */
};

enum xnuxx_boot_result {
    XNUXX_BOOT_OK = 0,
    XNUXX_BOOT_INVALID_ARGUMENT = -1,
    XNUXX_BOOT_INVALID_IMAGE = -2,
    XNUXX_BOOT_UNSUPPORTED = -3,
    XNUXX_BOOT_SIGNATURE_REJECTED = -4,
    XNUXX_BOOT_MEASUREMENT_REJECTED = -5,
    XNUXX_BOOT_ROLLBACK_REJECTED = -6,
    XNUXX_BOOT_CAPABILITY_REJECTED = -7,
    XNUXX_BOOT_LOAD_FAILED = -8,
    XNUXX_BOOT_RECOVERY_FAILED = -9
};

struct xnuxx_boot_platform {
    void *context;
    uint64_t capabilities;
    int ansi_console;
    int (*verify_signature)(void *, const void *, size_t, const void *, size_t);
    int (*measure_sha256)(void *, const void *, size_t, uint8_t[32]);
    int (*read_minimum_generation)(void *, uint64_t *);
    int (*load_kernel)(void *, const void *, size_t);
    int (*enter_recovery)(void *, enum xnuxx_boot_result);
    void (*write_console)(void *, const char *, size_t);
};

enum xnuxx_boot_result xnuxx_bootloader_start(
    const struct xnuxx_boot_platform *,
    const struct xnuxx_boot_image *,
    uint32_t policy);

#ifdef __cplusplus
}
#endif

#endif
