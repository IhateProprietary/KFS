ifeq ($(shell uname -s), Darwin)
	prefix := i386-elf-
endif

AS = nasm
ASFLAGS = -felf

LDFLAGS = -ffreestanding -O2 -nostdlib -lgcc

CFLAGS = -fno-builtin -fno-exceptions -fno-stack-protector \
		-nostdlib -nodefaultlibs \
		-std=gnu99 -ffreestanding -O2 -Wall -Wextra

arch ?= $(shell uname -m)
kernel := build/kernel-$(arch).bin

iso := build/os-$(arch).iso

linker_script := src/arch/$(arch)/linker.ld
grub_cfg := src/arch/$(arch)/grub.cfg

assembly_source_files := $(wildcard src/arch/$(arch)/asm/*.s)

assembly_object_files := $(patsubst src/arch/$(arch)/asm/%.s, \
	build/arch/$(arch)/%.o, $(assembly_source_files))


.PHONY: all clean re run iso


all: $(kernel)

clean:
	rm -r build

run: $(iso)
	qemu-system-x86_64 -cdrom $(iso)

re: clean all

iso: $(iso)

$(iso): $(kernel) $(grub_cfg)
	mkdir -p build/isofiles/boot/grub
	cp $(kernel) build/isofiles/boot/kernel.bin
	cp $(grub_cfg) build/isofiles/boot/grub
	$(prefix)grub-mkrescue -o $(iso) build/isofiles
	rm -r build/isofiles

$(kernel): $(assembly_object_files) $(linker_script)
	$(prefix)gcc -T $(linker_script) $(LDFLAGS) -o $(kernel) $(assembly_object_files)

# compile assembly files
build/arch/$(arch)/%.o: src/arch/$(arch)/%.s
	mkdir -p $(shell dirname $@)
	$(AS) $(ASFLAGS) $< -o $@
