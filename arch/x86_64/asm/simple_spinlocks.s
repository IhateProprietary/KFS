global simple_spin_lock
global simple_spin_unlock

section .text

simple_spin_lock:
		mov edx, [esp + 4]
.L1:
		mov eax, 1
		xchg eax, [edx]
		test eax, eax
		jnz .L1
		ret

simple_spin_unlock:
		mov edx, [esp + 4]
		xor eax, eax
		xchg eax, [edx]
		ret
