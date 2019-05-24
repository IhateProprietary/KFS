global simple_spin_lock
global simple_spin_unlock

section .text

simple_spin_lock:
	mov eax, 1
	xchg eax, [esp + 4]
	test eax, eax
	jnz simple_spin_lock
	ret

simple_spin_unlock:
	xor eax, eax
	xchg eax, [esp + 4]
	ret
