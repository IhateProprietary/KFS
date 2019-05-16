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

extern void _isr_errno(void);
extern void _isr_noerrno(void);

struct idt_entry idt_entries[256] = {
		[IDT_ISR_DIV] = INTGATE((u32)_isr0, IDT_KERNEL_CS, IDT_PVL_KERNEL),
		[IDT_ISR_DEBUG] = INTGATE((u32)_isr1, IDT_KERNEL_CS, IDT_PVL_KERNEL),
		[IDT_ISR_NMI] = INTGATE((u32)_isr2, IDT_KERNEL_CS, IDT_PVL_KERNEL),
		[IDT_ISR_BP] = INTGATE((u32)_isr3, IDT_KERNEL_CS, IDT_PVL_KERNEL),
		[IDT_ISR_OF] = INTGATE((u32)_isr4, IDT_KERNEL_CS, IDT_PVL_KERNEL),
		[IDT_ISR_BE] = INTGATE((u32)_isr5, IDT_KERNEL_CS, IDT_PVL_KERNEL),
		[IDT_ISR_INVOP] = INTGATE((u32)_isr6, IDT_KERNEL_CS, IDT_PVL_KERNEL),
		[IDT_ISR_DNA] = INTGATE((u32)_isr7, IDT_KERNEL_CS, IDT_PVL_KERNEL),
		[IDT_ISR_DF] = INTGATE((u32)_isr8, IDT_KERNEL_CS, IDT_PVL_KERNEL),
		[IDT_ISR_CSO] = INTGATE((u32)_isr9, IDT_KERNEL_CS, IDT_PVL_KERNEL),
		[IDT_ISR_INVTSS] = INTGATE((u32)_isr10, IDT_KERNEL_CS, IDT_PVL_KERNEL),
		[IDT_ISR_SNP] = INTGATE((u32)_isr11, IDT_KERNEL_CS, IDT_PVL_KERNEL),
		[IDT_ISR_SF] = INTGATE((u32)_isr12, IDT_KERNEL_CS, IDT_PVL_KERNEL),
		[IDT_ISR_GP] = INTGATE((u32)_isr13, IDT_KERNEL_CS, IDT_PVL_KERNEL),
		[IDT_ISR_PF] = INTGATE((u32)_isr14, IDT_KERNEL_CS, IDT_PVL_KERNEL),
		[IDT_ISR_FP] = INTGATE((u32)_isr16, IDT_KERNEL_CS, IDT_PVL_KERNEL),
		[IDT_ISR_AC] = INTGATE((u32)_isr17, IDT_KERNEL_CS, IDT_PVL_KERNEL),
		[IDT_ISR_MC] = INTGATE((u32)_isr18, IDT_KERNEL_CS, IDT_PVL_KERNEL),
		[IDT_ISR_SIMDFP] = INTGATE((u32)_isr19, IDT_KERNEL_CS, IDT_PVL_KERNEL),
		[IDT_ISR_VIRT] = INTGATE((u32)_isr20, IDT_KERNEL_CS, IDT_PVL_KERNEL),
};

struct idt _idtp = {
		.limit = sizeof(idt_entries),
		.entp = (u32)idt_entries
};

/* stack in reverse order? */
typedef struct
{
		u32		ds,
				edi, esi, ebp, esp, ebx, edx, ecx, eax,
				int_no, err_code,
				eip, cs, eflags, useresp, ss;
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
}
