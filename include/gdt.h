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

#ifndef __GDT_H__
# define __GDT_H__

# include "xstdint.h"

# define GDT_ACCESS_RW 0x02
# define GDT_ACCESS_DC 0x04
# define GDT_ACCESS_EX 0x08

# define GDT_ACCESS_DT 0x10
# define GDT_ACCESS_RING(x) (((x) & 3) << 5)
# define GDT_ACCESS_P 0x80

struct gdt_access_s
{
		/* Accessed bit. The CPU sets this to 1 when the segment is 
		   accessed. */
		u8	ac:1;
		/*
	     * Readable bit for code selectors: 
		 *	Whether read access for this segment is allowed.
		 *	Write access is never allowed for code segments.
		 *
		 * Writable bit for data selectors:
		 *	Whether write access for this segment is allowed.
		 *	Read access is always allowed for data segments.
		 */
		u8	rw:1;
		/*
		 * Direction bit for data selectors: Tells the direction.
		 * 0 the segment grows up. 1 the segment grows down 
		 *
		 * Conforming bit for code selectors:
		 *
		 * - If 1 code in this segment can be executed from an equal or 
		 * lower privilege level. For example, code in ring 3 can far-jump
		 * to conforming code in a ring 2 segment.
		 *
		 * - If 0 code in this segment can only be executed from the ring set in privl. 
		 */
		u8	dc:1;
		/* Executable bit. If 1 code in this segment can be executed, 
		   ie. a code selector. If 0 it is a data selector. */
		u8	ex:1;
		/* This bit should be set for code or data segments and should
		   be cleared for system segments (eg. a Task State Segment)  */
		u8	dt:1;
		/* Descriptor privilege level - Ring 0 - 3. */
		u8	dpl:2;
		/* Present bit. This must be 1 for all valid selectors. */
		u8	p:1;
};

union gdt_access_u
{
		u8				access;
		gdt_access_s	__access
};

# define GDT_GRAN_64BIT 0x20
# define GDT_GRAN_32BIT 0x40
# define GDT_GRAN_EXPND 0x80

struct gdt_granular_s
{
		/* Tells the maximum addressable unit */
		u8	limit_high:4;
		/* Hardware ignores the bit, so it's up to software how to use it. */
		u8	a:1;
		/* l must be zero if d == 1 and vice versa (1 = 64bit) */
		u8	l:1;
		/* Operand size (0 = 16bit, 1 = 32bit) */
		u8	d:1;
		/* Granularity (0 = 1 byte, 1 = 4kbyte) */
		u8	g:1;
};

typedef gdt_access_u gdt_access;

struct gdt_entry_s
{
		u16				limit_low;
		/* start */
		u16				base_low;
		u8				base_middle;
		gdt_access		access;
# define access access.access
# define access_ac access.__access.ac
# define access_rw access.__access.rw
# define access_dc access.__access.dc
# define access_ex access.__access.ex
# define access_dt access.__access.dt
# define access_dpl access.__access.dpl
# define access_p access.__access.p
		gdt_granular	__granular;
# define gran __granular
# define limit_high gran.limit_high
# define gran_avl gran.a
# define gran_long gran.l
# define gran_dword gran.d
# define gran_exp gran.g
		u8				base_high;
};

#endif /* __GDT_H__ */
