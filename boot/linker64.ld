ENTRY(_start)

SECTIONS
{
    . = 1M;

    .text : ALIGN(16)
    {
        KEEP(*(.multiboot))
        *(.text*)
    }

    .rodata : ALIGN(16)
    {
        *(.rodata*)
    }

    .data : ALIGN(16)
    {
        *(.data*)
    }

    .bss : ALIGN(16)
    {
        *(COMMON)
        *(.bss*)
    }
}
