#include <stdint.h>

void boot_platform_init(void);
void boot_platform_start(void);

void boot_entry(uint32_t magic, uint32_t multiboot_info)
{
    (void)multiboot_info;

    boot_platform_init();

    if (magic != 0x36D76289) {
        for (;;) {
            __asm__ volatile ("cli; hlt");
        }
    }

    boot_platform_start();

    for (;;) {
        __asm__ volatile ("cli; hlt");
    }
}
