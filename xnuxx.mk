CC=clang
LD=ld
BUILD=build
CFLAGS=-m32 -ffreestanding -fno-pic -fno-stack-protector -fno-builtin -fno-strict-aliasing -Wall -Wextra -Iinclude
LDFLAGS=-m elf_i386 -T boot/linker.ld

all: $(BUILD)/xnuxx.elf

$(BUILD):
mkdir -p $(BUILD)

$(BUILD)/boot_entry.o: boot/boot_entry.c | $(BUILD)
$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/boot_platform.o: boot/boot_platform.c | $(BUILD)
$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/string.o: boot/string.c | $(BUILD)
$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/kernel.o: kernel/kernel.c | $(BUILD)
$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/multiboot2.o: boot/multiboot2.S | $(BUILD)
$(CC) -m32 -x assembler-with-cpp -c $< -o $@

$(BUILD)/xnuxx.elf: $(BUILD)/multiboot2.o $(BUILD)/boot_entry.o $(BUILD)/boot_platform.o $(BUILD)/string.o $(BUILD)/kernel.o
$(LD) $(LDFLAGS) -o $@ $^

clean:
rm -rf $(BUILD)

.PHONY: all clean
