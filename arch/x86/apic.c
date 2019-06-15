/*
 *  This file is part of Kernel From Scratch.
 *
 *  Kernel From Scratch is free software: you can redistribute it and/or modify
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

#include "kernel.h"
#include "xstdint.h"
#include "msr.h"

void test_apic(void)
{
	u32 high, low;
	u32 apic_base;

	rdmsr(0x1b, high, low);
	printk("msr 1b read 0x%x%x\n", high, low);

	apic_base = low & 0xffff0000;
	printk("msr 0x%x\n", *(volatile u32 *)(apic_base+0x20));

	*((volatile u32*)(0xfec00000)) = 0x1;
	printk("ioapic id 0x%x\n", *(volatile u32 *)(0xfec00000 + 0x10));
	*((volatile u32*)(0xfec00000)) = 0x2;
	printk("ioapic id 0x%x\n", *(volatile u32 *)(0xfec00000 + 0x10));
	printk("null == 0x%x\n", *(volatile u32 *)(0));
}
