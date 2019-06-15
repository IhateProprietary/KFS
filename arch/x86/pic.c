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

#include "io.h"
#include "xstdint.h"

/*
 * 0x20 == PIC master cmd port
 * 0x21 == PIC master data port
 * 0xa0 == PIC slave cmd port
 * 0xa1 == PIC slave data port
 */

/*
 * This functions disable PIC8259 by masking them
 */
void pic_disable(void)
{
	outb(0x20, 0x11);
	outb(0xa0, 0x11);

	outb(0x21, 0xe0);
	outb(0xa1, 0xe8);

	outb(0x21, 4);
	outb(0xa1, 2);

	outb(0x21, 0xff);
	outb(0xa1, 0xff);
}

void pic_enable(const u8 master_mask, const u8 slave_mask)
{
	outb(0x20, 0x11);
	outb(0xa0, 0x11);

	outb(0x21, 0xe0);
	outb(0xa1, 0xe8);

	outb(0x21, 4);
	outb(0xa1, 2);

	outb(0x21, master_mask);
	outb(0xa1, slave_mask);
}
