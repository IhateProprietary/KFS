
;; check if CPU has cpuid otherwise skip
	call __has_cpuid
	test eax, eax
	jnz .multiboot2

;; get cpu info
	mov eax, 1
	mov [_cpuid_avail], eax

	push ebx

	mov eax, 0
	cpuid
	mov [_cpuvendor], ebx
	mov [_cpuvendor + 4], edx
	mov [_cpuvendor + 8], ecx
	mov eax, 1
	cpuid
	mov [_cpumodel], eax
	mov [_cpu_feat_ebx], ebx
	mov [_cpu_feat_ecx], ecx
	mov [_cpu_feat_edx], edx

	pop ebx

	push _cpuvendor
	push _fmt2
	call printk
	mov esp, ebp
.multiboot2:
