ifeq ($(shell uname -s), Darwin)
	CC := i386-elf-gcc
	GRUB := i386-elf-grub
	AR := i386-elf-ar
else
	CC := i686-elf-gcc
	GRUB := grub
	AR := ar
endif

AS = nasm
ASFLAGS = -felf

LDFLAGS = -ffreestanding -O2 -nostdlib -lgcc

CFLAGS = -fno-builtin -fno-exceptions -fno-stack-protector \
		-nostdlib -nodefaultlibs  \
		-std=gnu99 -ffreestanding -m32 -Wall -Wextra -g3 

arch = x86

kernel := build/kernel-$(arch).bin

iso := build/os-$(arch).iso

linker_script := arch/$(arch)/linker.ld
grub_cfg := arch/$(arch)/grub.cfg

assembly_source_files := $(wildcard arch/$(arch)/asm/*.s)
c_source_files := $(wildcard arch/$(arch)/*.c)

assembly_object_files := $(patsubst arch/$(arch)/asm/%.s, \
	build/arch/$(arch)/asm/%.o, $(assembly_source_files))
c_object_files := $(patsubst arch/$(arch)/%.c, \
	build/arch/$(arch)/%.o, $(c_source_files))

stringop := build/libsops.a
printk := build/printk.o

.PHONY: all clean re run iso

all: $(kernel)

clean:
	make -C stringop clean
	rm -rf build

run: $(iso)
	qemu-system-x86_64 -cdrom $(iso)

re: clean all

iso: $(iso)

$(iso): $(kernel) $(grub_cfg)
	@mkdir -p build/isofiles/boot/grub
	cp $(kernel) build/isofiles/boot/kernel.bin
	cp $(grub_cfg) build/isofiles/boot/grub
	$(GRUB)-mkrescue -o $(iso) build/isofiles
	rm -r build/isofiles

$(kernel): $(assembly_object_files) $(c_object_files) $(linker_script) $(stringop) $(printk)
	$(CC) -T $(linker_script) $(LDFLAGS) -o $(kernel) $(assembly_object_files) \
					$(printk) $(c_object_files) -Lbuild -lsops

$(stringop):
	make -C stringop CFLAGS+="$(CFLAGS)" AR=$(AR) CC+=$(CC) TARGET=../$@

$(printk):
	make -C printk CFLAGS+="$(CFLAGS)" CC+=$(CC) TARGET=../$@

# compile assembly files
build/arch/$(arch)/asm/%.o: arch/$(arch)/asm/%.s
	@mkdir -p $(shell dirname $@)
	$(AS) $(ASFLAGS) $< -o $@

build/arch/$(arch)/%.o: arch/$(arch)/%.c
	@mkdir -p $(shell dirname $@)
	$(CC) $(CFLAGS) -c $< -o $@ -Iinclude
