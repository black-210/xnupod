.set MULTIBOOT2_MAGIC, 0xE85250D6
.set ARCHITECTURE, 0

.section .multiboot
.align 8
.long MULTIBOOT2_MAGIC
.long ARCHITECTURE
.long (multiboot_header_end - multiboot_header)
.long -(MULTIBOOT2_MAGIC + ARCHITECTURE + (multiboot_header_end - multiboot_header))

/* end tag */
.word 0
.word 0
.long 8

multiboot_header_end:

.section .bss
.align 16
stack_bottom:
.skip 16384
stack_top:

.section .text
.global _start
.type _start, @function

_start:
    mov $stack_top, %esp

    push %ebx
    push %eax

    call boot_entry

1:
    cli
    hlt
    jmp 1b
