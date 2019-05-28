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

#include "simple_spin_lock.h"
#include "minimal_stringop.h"
#include "minimal_vga_tty.h"
#include "xstdint.h"

#define VGA_HEIGHT 80
#define VGA_WIDTH 25

#define __vga_putchar(c)									\
		*(vga_buffer + (VGA_WIDTH * cursey) + cursex++) = (c)

static u16 *vga_buffer = (u16 *)0xb8000;
static u16 cursex = 0;
static u16 cursey = 0;

static _spin_lock_t _vgalock = 0;

/*
 * Scroll up by one, and clears "cache" from vga buffer
 */

static inline void __vga_scroll_up(void)
{
		_memmove(vga_buffer, vga_buffer + (VGA_WIDTH),					\
				 ((VGA_HEIGHT - 1) * VGA_WIDTH) * sizeof(*vga_buffer));

		_memset(vga_buffer + VGA_WIDTH * (VGA_HEIGHT - 1), 0, VGA_WIDTH);
}

ssize_t __vga_write(const void *mem, u8 color, size_t size)
{
		register u16 _vgac = color << 8;
		register u32 idx;
		const u8 *zone = mem;

		simple_spin_lock(&_vgalock);
		for (idx = 0; idx < size; ++idx)
		{
				switch (idx[zone]) {
				case 0xa:
						++cursey;
						cursex = 0;
						break ;
				default:
						_vgac = (_vgac & ~0xff) | idx[zone];
						__vga_putchar(_vgac);
				}
				if (cursex >= VGA_WIDTH) {
						cursex = 0;
						++cursey;
				}
				if (cursey >= VGA_HEIGHT) {
						__vga_scroll_up();
						--cursey;
				}
		}
		simple_spin_unlock(&_vgalock);
		return size;
}
