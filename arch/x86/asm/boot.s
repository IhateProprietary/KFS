%define MAGIC 0xE85250D6

section .multiboot_header
__multi_header_start:
;; multiboot header 
	dd MAGIC
;; 0 for x86 4 for MIPS (MCU thing)
	dd 0
;; size
	dd __multi_header_end - __multi_header_start
;; checksum
	dd -(MAGIC + 0 + (__multi_header_end - __multi_header_start))

;; header must end with this sequence
;; tag
	dw 0
;; flag
	dw 0
;; size
	dd 8
__multi_header_end:
section .bss
	; 4096 * 4 (dual word)
	; 16Kbyte stack
_stack_top:
	resd 4096
_stack_bottom:

section .data
global _sysinfo
global _cpuinfo

_sysinfo:
_acpi_rsdp: dd 0
_frameinfo: dd 0
_elfsection: dd 0
_mmap: dd 0
_meminfo: dd 0

;; ... more to come maybe
_lapic_addr: dd 0
_ioapic_addr: dd 0
_facp_addr: dd 0
_dsdt_addr: dd 0

section .text
global _start

extern _gdtp
extern gdt_flush

extern printk

extern _idtp
extern idt_init
extern idt_flush

extern _set_sysinfo
extern _set_apic_addr

extern __rsdp_ok
extern _find_sdt

extern pic_enable
extern pic_disable

extern test_apic
;; the "main"
_start:
;; The OS image must create its own stack as soon as it needs one.
;; Multiboot 2 Chaper 3.3
	mov esp, _stack_bottom
	mov ebp, esp

.multiboot2:
;; Parsing multiboot2 BIOS info
	push _sysinfo
	push ebx
	call _set_sysinfo
	mov esp, ebp

;; checking ACPI table is OK
	push DWORD [_acpi_rsdp]
	call __rsdp_ok
	test eax, eax
	jnz .fail

	push _APIC_s
	mov edx, [ebp-4] 		;_acpi_rsdp
	mov eax, [edx+24]		;offset to sdt_p
	push eax
	call _find_sdt
	mov esp, ebp
;; test if ACPI has APIC table or not
	test eax, eax
	jz .gdt

	push _sysinfo
	push eax
	call _set_apic_addr
	mov esp, ebp

.gdt:
;; initialize GDT
	mov eax, _gdtp
	push eax
	call gdt_flush

	push _gdtp
	push _gdt_ok
	push _fmt
	call printk
	mov esp, ebp

;; initialize IDT
	call idt_init
	push _idtp
	call idt_flush
	mov esp, ebp

	push _idtp
	push _idt_ok
	push _fmt
	call printk
	mov esp, ebp

	call pic_disable
	call test_apic

;; testing exception IDT
	int 0
	jmp .ok
.fail:
	push _fail
	call printk
.ok:
	hlt
	jmp .ok
;; triple fault if somehow goes past here
	ud2

section .rodata
_fmt2: db "cpu vendor %.12s", 0xa, 0
_fail: db "Some error happened", 0xa, 0
_fmt: db "%s  0x%x", 0xa, 0
_gdt_ok: db "GDT ", 0xe, "f4OK", 0xe, "f7", 0
_idt_ok: db "IDT OK", 0
_APIC_s: db "APIC"
