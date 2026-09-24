#include <xnu++/bootloader.h>

static void
console_write(void *context, const char *text, size_t length)
{
    volatile unsigned char *vga = (volatile unsigned char *)0xB8000;

    (void)context;

    for (size_t i = 0; i < length; i++) {
        vga[i * 2] = (unsigned char)text[i];
        vga[i * 2 + 1] = 0x1F;
    }
}

static int
verify_signature(void *context,
    const void *payload,
    size_t payload_size,
    const void *signature,
    size_t signature_size)
{
    (void)context;
    (void)payload;
    (void)payload_size;
    (void)signature;
    (void)signature_size;

    /*
     * Temporary VM bring-up implementation.
     * Replace with real signature verification.
     */
    return 0;
}

static int
measure_sha256(void *context,
    const void *payload,
    size_t payload_size,
    uint8_t measurement[32])
{
    (void)context;
    (void)payload;
    (void)payload_size;

    for (int i = 0; i < 32; i++)
        measurement[i] = 0;

    return 0;
}

static int
read_generation(void *context, uint64_t *generation)
{
    (void)context;

    *generation = 0;
    return 0;
}

static int
load_kernel(void *context,
    const void *payload,
    size_t payload_size)
{
    (void)context;
    (void)payload;
    (void)payload_size;

    /*
     * Real kernel handoff goes here.
     */
    return 0;
}

static int
enter_recovery(void *context, enum xnuxx_boot_result result)
{
    (void)context;
    (void)result;

    return -1;
}

void
boot_platform_init(void)
{
}

void
boot_platform_start(void)
{
    static const uint8_t measurement[32] = {0};

    static const struct xnuxx_boot_image image = {
        .magic = XNUXX_BOOTLOADER_MAGIC,
        .version = XNUXX_BOOTLOADER_API_VERSION,
        .header_size = sizeof(struct xnuxx_boot_image),
        .generation = 0,
        .required_capabilities = 0,
        .payload = (const void *)0x100000,
        .payload_size = 4096,
        .signature = "VM",
        .signature_size = 2,
        .expected_measurement = measurement
    };

    static const struct xnuxx_boot_platform platform = {
        .context = 0,
        .capabilities =
            XNUXX_BOOT_CAP_KERNEL_PROVIDER |
            XNUXX_BOOT_CAP_SECURITY_LAYER |
            XNUXX_BOOT_CAP_UPDATE_LAYER |
            XNUXX_BOOT_CAP_DEVICE_LAYER |
            XNUXX_BOOT_CAP_RECOVERY,

        .ansi_console = 0,
        .verify_signature = verify_signature,
        .measure_sha256 = measure_sha256,
        .read_minimum_generation = read_generation,
        .load_kernel = load_kernel,
        .enter_recovery = enter_recovery,
        .write_console = console_write
    };

    xnuxx_bootloader_start(
        &platform,
        &image,
        XNUXX_BOOT_REQUIRE_SIGNATURE |
        XNUXX_BOOT_REQUIRE_MEASUREMENT |
        XNUXX_BOOT_REQUIRE_ROLLBACK_PROTECTION |
        XNUXX_BOOT_REQUIRE_LAYER_CAPABILITIES
    );
}
