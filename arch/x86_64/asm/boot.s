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
_stack_top:
	resd 4096
_stack_bottom:

section .text
global _start
extern _gdtp
extern gdt_flush
bits 32
	; the "main"
_start:
	mov dword [SCREEN], 0x2f4b2f4f
	mov esp, _stack_bottom
	mov eax, _gdtp
	push eax
	call gdt_flush
	mov dword [SCREEN+60], 0x2f4b2f4f
.L1:
	hlt
	jmp .L1
