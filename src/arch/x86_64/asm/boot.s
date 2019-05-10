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
	dd 4096
global _start
section .text
bits 32
	; the "main"
_start:
	mov dword [SCREEN], 0x2f4b2f4f
	hlt
