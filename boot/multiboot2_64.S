.set MULTIBOOT2_MAGIC, 0xE85250D6
.set ARCHITECTURE, 0

.section .multiboot
.align 8
.long MULTIBOOT2_MAGIC
.long ARCHITECTURE
.long 24
.long 0x17ADAFD2
.word 0
.word 0
.long 8

.section .text
.code32

.global _start
.type _start,@function

_start:
    cli

    movl %eax, multiboot_magic
    movl %ebx, multiboot_info

    movl $stack_top32, %esp
    xorl %ebp, %ebp

    # PML4 -> PDPT
    movl $boot_pdpt, %eax
    orl $0x3, %eax
    movl %eax, boot_pml4

    # PDPT -> PD
    movl $boot_pd, %eax
    orl $0x3, %eax
    movl %eax, boot_pdpt

    # Identity-map first 1 GiB using 2 MiB pages
    xorl %eax, %eax
    movl $boot_pd, %edi
    movl $512, %ecx

1:
    movl %eax, (%edi)
    orl $0x83, (%edi)
    addl $0x200000, %eax
    addl $8, %edi
    loop 1b

    # Load PML4
    movl $boot_pml4, %eax
    movl %eax, %cr3

    # Enable PAE
    movl %cr4, %eax
    orl $0x20, %eax
    movl %eax, %cr4

    # Enable EFER.LME
    movl $0xC0000080, %ecx
    rdmsr
    orl $0x100, %eax
    wrmsr

    # Enable paging
    movl %cr0, %eax
    orl $0x80000000, %eax
    movl %eax, %cr0

    # Load 64-bit GDT
    lgdt gdt64_descriptor

    # Enter long mode
    ljmp $0x08, $long_mode_entry

.code64

long_mode_entry:
    movw $0x10, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %ss

    movq $stack_top64, %rsp
    xorq %rbp, %rbp

    # SysV x86-64 C arguments:
    # boot_entry(uint32_t magic, uint32_t multiboot_info)
    movl multiboot_magic(%rip), %edi
    movl multiboot_info(%rip), %esi

    call boot_entry

1:
    cli
    hlt
    jmp 1b

.section .rodata

.align 8
gdt64:
    .quad 0x0000000000000000
    .quad 0x00AF9A000000FFFF
    .quad 0x00AF92000000FFFF

gdt64_descriptor:
    .word gdt64_descriptor - gdt64 - 1
    .quad gdt64

.section .bss

.align 4096
boot_pml4:
    .skip 4096

boot_pdpt:
    .skip 4096

boot_pd:
    .skip 4096

.align 16
stack32:
    .skip 16384
stack_top32:

.align 16
stack64:
    .skip 16384
stack_top64:

.align 4
multiboot_magic:
    .long 0

multiboot_info:
    .long 0
