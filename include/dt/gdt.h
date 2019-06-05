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
# define GDT_PVL_KERNEL 0
# define GDT_PVL_USER 3
# define GDT_ACCESS_RING(x) (((x) & 3) << 5)
# define GDT_ACCESS_P 0x80

/* 
 * ac 
 * Accessed bit. The CPU sets this to 1 when the segment is
 * accessed. 
 *
 * rw
 * Readable bit for code selectors: 
 *	Whether read access for this segment is allowed.
 *	Write access is never allowed for code segments.
 * Writable bit for data selectors:
 *	Whether write access for this segment is allowed.
 *	Read access is always allowed for data segments.
 *
 * dc
 * Direction bit for data selectors: Tells the direction.
 * 0 the segment grows up. 1 the segment grows down 
 * Conforming bit for code selectors:
 * - If 1 code in this segment can be executed from an equal or 
 * lower privilege level. For example, code in ring 3 can far-jump
 * to conforming code in a ring 2 segment.
 * - If 0 code in this segment can only be executed from the ring set in privl.
 *
 * ex
 * Executable bit. If 1 code in this segment can be executed, 
 * ie. a code selector. If 0 it is a data selector.
 *
 * dt
 * This bit should be set for code or data segments and should
 * be cleared for system segments (eg. a Task State Segment) 
 *
 * dpl
 * Descriptor privilege level - Ring 0 - 3.
 *
 * p
 * Present bit. This must be 1 for all valid selectors.
 */

struct gdt_access_bits
{
	u8	ac:1,
		rw:1,
		dc:1,
		ex:1,
		dt:1,
		dpl:2,
		p:1;
}  __attribute__((packed));

typedef union gdt_access_u
{
	u8			access;
	struct gdt_access_bits	bits;
} __attribute__((packed)) gdt_access_t;

# define GDT_GRAN_64BIT 0x20
# define GDT_GRAN_32BIT 0x40
# define GDT_GRAN_EXPND 0x80

/*
 * limit_high
 * Tells the maximum addressable unit
 *
 * a
 * Hardware ignores the bit, so it's up to software how to use it.
 *
 * l / d
 * l == 64bit / d == 32bit if both 0 -> 16bit
 *
 * g
 * Granularity (0 = 1 byte, 1 = 4kbyte)
 */

struct gdt_granular_bits
{
	u8	limit_high:4,
		a:1,
		l:1,
		d:1,
		g:1;
} __attribute__((packed));

typedef union gdt_granular_u
{
	u8				gran;
	struct gdt_granular_bits	bits;
		
} __attribute__((packed)) gdt_granular_t;

struct gdt_entry
{
	u16		limit_low;
	u16		base_low;
	u8		base_middle;
	gdt_access_t	access;
# define gdt_access	access.access
# define gdt_ac		access.bits.ac
# define gdt_rw		access.bits.rw
# define gdt_dc		access.bits.dc
# define gdt_ex		access.bits.ex
# define gdt_dt		access.bits.dt
# define gdt_dpl	access.bits.dpl
# define gdts_p		access.bits.p
	gdt_granular_t	gran;
# define gdt_gran	gran.gran
# define limit_high 	gran.bits.limit_high
# define gdt_avl	gran.bits.a
# define gdt_qword	gran.bits.l
# define gdt_dword	gran.bits.d
# define gdt_exp	gran.bits.g
	u8		base_high;
} __attribute__((packed));

typedef struct gdt_entry* gdt_entryp;

struct gdt
{
	u16 limit;
	u32 entp;
} __attribute__((packed));

extern struct gdt_entry	gdt_entries[];
extern struct gdt	gdtp;

extern void gdt_flush(struct gdt *);

#endif /* __GDT_H__ */
