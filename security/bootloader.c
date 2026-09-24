#include <xnu++/bootloader.h>

#include <string.h>

#define ANSI_BLUE_WHITE "\033[44;37m"
#define ANSI_RESET "\033[0m"

static void
boot_log(const struct xnuxx_boot_platform *p, const char *text)
{
    if (p->write_console != NULL) {
        p->write_console(p->context, text, strlen(text));
    }
}

static void
boot_fail(const struct xnuxx_boot_platform *p, enum xnuxx_boot_result result)
{
    if (p->enter_recovery != NULL && p->enter_recovery(p->context, result) == 0) {
        return;
    }
}

static enum xnuxx_boot_result
boot_reject(const struct xnuxx_boot_platform *p, enum xnuxx_boot_result result)
{
    boot_fail(p, result);
    return result;
}

enum xnuxx_boot_result
xnuxx_bootloader_start(const struct xnuxx_boot_platform *p,
    const struct xnuxx_boot_image *image, uint32_t policy)
{
    uint8_t actual_measurement[32];
    uint64_t minimum_generation;
    uint64_t missing_capabilities;
    int verified;

    if (p == NULL || image == NULL || image->payload == NULL ||
        image->payload_size == 0 || image->payload_size > XNUXX_BOOTLOADER_MAX_IMAGE_SIZE ||
        p->write_console == NULL) {
        return XNUXX_BOOT_INVALID_ARGUMENT;
    }

    if (p->ansi_console) {
        boot_log(p, ANSI_BLUE_WHITE);
    }
    boot_log(p, "xnu++ secure bootloader\n");

    if (image->magic != XNUXX_BOOTLOADER_MAGIC ||
        image->version != XNUXX_BOOTLOADER_API_VERSION ||
        image->header_size < sizeof(*image) ||
        image->signature == NULL || image->signature_size == 0 ||
        image->expected_measurement == NULL) {
        return boot_reject(p, XNUXX_BOOT_INVALID_IMAGE);
    }

    if ((policy & XNUXX_BOOT_REQUIRE_LAYER_CAPABILITIES) != 0) {
        missing_capabilities = image->required_capabilities & ~p->capabilities;
        if (missing_capabilities != 0) {
            return boot_reject(p, XNUXX_BOOT_CAPABILITY_REJECTED);
        }
    }

    if ((policy & XNUXX_BOOT_REQUIRE_SIGNATURE) == 0 || p->verify_signature == NULL) {
        return boot_reject(p, XNUXX_BOOT_SIGNATURE_REJECTED);
    }
    verified = p->verify_signature(p->context, image->payload, image->payload_size,
        image->signature, image->signature_size);
    if (verified != 0) {
        return boot_reject(p, XNUXX_BOOT_SIGNATURE_REJECTED);
    }

    if ((policy & XNUXX_BOOT_REQUIRE_MEASUREMENT) == 0 || p->measure_sha256 == NULL) {
        return boot_reject(p, XNUXX_BOOT_MEASUREMENT_REJECTED);
    }
    if (p->measure_sha256(p->context, image->payload, image->payload_size,
            actual_measurement) != 0 ||
        memcmp(actual_measurement, image->expected_measurement,
            sizeof(actual_measurement)) != 0) {
        memset(actual_measurement, 0, sizeof(actual_measurement));
        return boot_reject(p, XNUXX_BOOT_MEASUREMENT_REJECTED);
    }
    memset(actual_measurement, 0, sizeof(actual_measurement));

    if ((policy & XNUXX_BOOT_REQUIRE_ROLLBACK_PROTECTION) == 0 ||
        p->read_minimum_generation == NULL ||
        p->read_minimum_generation(p->context, &minimum_generation) != 0 ||
        image->generation < minimum_generation) {
        return boot_reject(p, XNUXX_BOOT_ROLLBACK_REJECTED);
    }

    if (p->load_kernel == NULL ||
        p->load_kernel(p->context, image->payload, image->payload_size) != 0) {
        return boot_reject(p, XNUXX_BOOT_LOAD_FAILED);
    }

    boot_log(p, "secure verification complete; transferring control\n");
    if (p->ansi_console) {
        boot_log(p, ANSI_RESET);
    }
    return XNUXX_BOOT_OK;
}
