/*
 *  This file is part of Kernel From Scratch.
 *
 *	Kernel From Scratch is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "dt/idt.h"
#include "kernel.h"

extern void _isr0(void);
extern void _isr1(void);
extern void _isr2(void);
extern void _isr3(void);
extern void _isr4(void);
extern void _isr5(void);
extern void _isr6(void);
extern void _isr7(void);
extern void _isr8(void);
extern void _isr9(void);
extern void _isr10(void);
extern void _isr11(void);
extern void _isr12(void);
extern void _isr13(void);
extern void _isr14(void);
extern void _isr16(void);
extern void _isr17(void);
extern void _isr18(void);
extern void _isr19(void);
extern void _isr20(void);
extern void _undefined(void);

struct idt_entry idt_entries[256] = {
		
};

struct idt _idtp = {
		.limit = sizeof(idt_entries) - 1,
		.entp = (u32)idt_entries
};

void idt_init(void)
{
		static void (*_isr[IDT_ISR_USR_GATE])(void) = {
				[IDT_ISR_DIV] = _isr0,
				[IDT_ISR_DEBUG] = _isr1,
				[IDT_ISR_NMI] = _isr2,
				[IDT_ISR_BP] = _isr3,
				[IDT_ISR_OF] = _isr4,
				[IDT_ISR_BE] = _isr5,
				[IDT_ISR_INVOP] =  _isr6,
				[IDT_ISR_DNA] = _isr7,
				[IDT_ISR_DF] = _isr8,
				[IDT_ISR_CSO] = _isr9,
				[IDT_ISR_INVTSS] = _isr10,
				[IDT_ISR_SNP] = _isr11,
				[IDT_ISR_SF] = _isr12,
				[IDT_ISR_GP] = _isr13,
				[IDT_ISR_PF] = _isr14,
				[IDT_ISR_FP] = _isr16,
				[IDT_ISR_AC] = _isr17,
				[IDT_ISR_MC] = _isr18,
				[IDT_ISR_SIMDFP] = _isr19,
				[IDT_ISR_VIRT] = _isr20,
		};
		register int n;
		register u32 base_ptr;

		for (n = 0; n < IDT_ISR_USR_GATE; n++)
		{
				base_ptr = (u32)(n[_isr] ? n[_isr] : _undefined);
				idt_entries[n] = (struct idt_entry)INTGATE(base_ptr, IDT_KERNEL_CS, IDT_PVL_KERNEL);
		}
}

/* stack in reverse order? */
typedef struct
{
		u32		ss, gs ,fs, es, ds,
				edi, esi, ebp, esp, ebx, edx, ecx, eax,
				int_no, err_code,
				eip, cs, eflags, useresp, userss;
} reg_saved_state_t;

void __isr_fault_handler(reg_saved_state_t state)
{
		static const char *strerr[IDT_ISR_USR_GATE] = {
				[IDT_ISR_DIV] = "Divide Error",
				[IDT_ISR_DEBUG] = "Debug exception",
				[IDT_ISR_NMI] = "NMI Exception",
				[IDT_ISR_BP] = "Breakpoint Exception",
				[IDT_ISR_OF] = "Overflow Exception",
				[IDT_ISR_BE] = "Bound Range Exceeded Exception",
				[IDT_ISR_INVOP] =  "Invalid Instruction",
				[IDT_ISR_DNA] = "Device not available",
				[IDT_ISR_DF] = "Double Fault Exception",
				[IDT_ISR_CSO] = "Coprocessor segment overrun",
				[IDT_ISR_INVTSS] = "Invalid TSS exception",
				[IDT_ISR_SNP] = "Segment not present",
				[IDT_ISR_SF] = "Stack Fault exception",
				[IDT_ISR_GP] = "General protection exception",
				[IDT_ISR_PF] = "Page fault exception",
				[IDT_ISR_FP] = "Floating point unit error",
				[IDT_ISR_AC] = "Alignment check eception",
				[IDT_ISR_MC] = "Machine check exception",
				[IDT_ISR_SIMDFP] = "SIMD floating point exception",
				[IDT_ISR_VIRT] = "Virtualization exception",
		};
		const char *_errorlog = "Unregistered Interrupt Handler";

		if (strerr[state.int_no])
				_errorlog = strerr[state.int_no];

		printk("Interrupt: 0x%x Error: %s\n", state.int_no, _errorlog);
}
