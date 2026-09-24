#include <xnu++/bootloader.h>

static void
console_write(void *context, const char *text, size_t length)
{
    volatile unsigned char *vga =
        (volatile unsigned char *)0xB8000;

    size_t pos = 0;

    (void)context;

    for (size_t i = 0; i < length; i++) {
        if (text[i] == '\n') {
            pos += 80 - (pos % 80);
            continue;
        }

        vga[pos * 2] = (unsigned char)text[i];
        vga[pos * 2 + 1] = 0x1F;
        pos++;

        if (pos >= 80 * 25)
            break;
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
     * VM bring-up only.
     * Replace with real cryptographic verification.
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

    for (size_t i = 0; i < 32; i++)
        measurement[i] = 0;

    /*
     * VM bring-up only.
     * Replace with real SHA-256.
     */
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
     * Kernel handoff is implemented in the next stage.
     */
    return 0;
}

static int
enter_recovery(void *context,
    enum xnuxx_boot_result result)
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
    static const uint8_t measurement[32] = { 0 };

    static const char signature[] = "VM";

    static const struct xnuxx_boot_image image = {
        .magic = XNUXX_BOOTLOADER_MAGIC,
        .version = XNUXX_BOOTLOADER_API_VERSION,
        .header_size = sizeof(struct xnuxx_boot_image),
        .generation = 0,
        .required_capabilities =
            XNUXX_BOOT_CAP_KERNEL_PROVIDER |
            XNUXX_BOOT_CAP_SECURITY_LAYER |
            XNUXX_BOOT_CAP_DEVICE_LAYER,
        .payload = (const
