%define MAGIC 0xE85250D6

%define SCREEN 0xB8000

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

section .text
global _start

extern _gdtp
extern gdt_flush

extern printk

extern _idtp
extern idt_init
extern idt_flush

;; the "main"

_start:
		mov esp, _stack_bottom
		mov ebp, esp

		mov eax, _gdtp
		push eax
		call gdt_flush

		push _gdtp
		push _gdt_ok
		push _fmt
		call printk
		mov esp, ebp

		call idt_init

		push _idtp
		call idt_flush
		mov esp, ebp

		push _idtp
		push _idt_ok
		push _fmt
		call printk
		mov esp, ebp

		int 0x00
.L1:
		hlt
		jmp .L1

section .rodata
_fmt: db "%s  0x%x", 0xa, 0
_gdt_ok: db "GDT OK", 0
_idt_ok: db "IDT OK", 0
