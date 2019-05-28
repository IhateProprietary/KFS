section .text

%macro noerr 1
global _isr%1
		_isr%1:
		push BYTE 0
		push BYTE %1
		jmp isr_common_stub
%endmacro

%macro err 1
global _isr%1
		_isr%1:
		push BYTE %1
		jmp isr_common_stub
%endmacro

		noerr 0
		noerr 1
		noerr 2
		noerr 3
		noerr 4
		noerr 5
		noerr 6
		noerr 7
		err 8
		noerr 9
		err 10
		err 11
		err 12
		err 13
		err 14
		noerr 16
		err 17
		noerr 18
		noerr 19
		noerr 20

global _undefined
_undefined:
		push BYTE 0
		push BYTE -1
		jmp isr_common_stub

extern __isr_fault_handler

isr_common_stub:
		pusha

		push ds
		push es
		push fs
		push gs
		push ss

		mov ax, 0x10
		mov ds, ax
		mov es, ax
		mov fs, ax
		mov gs, ax
		mov ax, 0x18
		mov ss, ax
		call __isr_fault_handler
		pop ss
		pop gs
		pop fs
		pop es
		pop ds
		popa
		iret
