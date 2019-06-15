section .text

global __has_cpuid
__has_cpuid:
	push ebp
	mov ebp, esp

	pushfd                      ;Save EFLAGS
	pushfd                      ;Store EFLAGS
	mov edx, [esp]		    ;save original into edx for checking later
	xor dword [esp],0x00200000  ;Invert the ID bit in stored EFLAGS
	popfd                       ;Load stored EFLAGS (with ID bit inverted)
	pushfd                      ;Store EFLAGS again (ID bit may or may not be inverted)
	pop eax                     ;eax = modified EFLAGS (ID bit may or may not be inverted)
	popfd			    ;restore original eflag

	cmp edx, eax		    ;if no change, no cpuid
	jz .no_cpuid

	mov eax, 0
	jmp .end
.no_cpuid:
	mov eax, 1
.end
	mov esp, ebp
	pop ebp
	ret
