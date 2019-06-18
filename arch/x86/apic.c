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

#define sti() __asm__ __volatile__("sti")
#define IOREGSEL 0xfec00000
#define IOWIN    0xfec00010

u32 read_ioapic_reg(volatile u32 *ioapic_baddr, u32 __reg)
{
	*ioapic_baddr = __reg;
	return 4[ioapic_baddr];
}

void write_ioapic_reg(volatile u32 *ioapic_baddr, u32 __reg, u32 val)
{
	*ioapic_baddr = __reg;
	4[ioapic_baddr] = val;
}


#if 0
/* 
 * the following function should just activate PIT
 * on the pin 2 of IO APIC
 * 2 known pin is 1, for HID inputs, and 2 for PIT,
 * other are unknown or reprogrammed by kernel
 */
void test_apic()
{
	u32 regval;
	u32 apic_base;

	/* high */
	*((volatile u32*)(IOREGSEL)) = 0x15;
	regval = *((volatile u32*)(IOWIN));
	regval = (regval & ~(0xff000000));
	*((volatile u32*)(IOWIN)) = regval;

	/* low */
	*((volatile u32*)(IOREGSEL)) = 0x14;
	regval = *((volatile u32*)(IOWIN));
	regval = (regval & ~(0x10FFF)) | 0x20;
	*((volatile u32*)(IOWIN)) = regval;

	sti();
}
#endif
