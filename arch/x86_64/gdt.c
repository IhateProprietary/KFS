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

#include "dt/gdt.h"

struct gdt_entry	gdt_entries[GDT_NENTRY] = {
		{0, 0, 0, {0}, {0}, 0},
		/* Kernel code segment */
		(struct gdt_entry){
				.limit_low = 0xFFFF,
				.base_low = 0,
				.base_middle = 0,
				.base_high = 0,
				.gdt_access = (GDT_ACCESS_RW | GDT_ACCESS_EX | GDT_ACCESS_P | \
							   GDT_ACCESS_RING(GDT_PVL_KERNEL) |			  \
							   GDT_ACCESS_DT),
				.gdt_gran = (GDT_GRAN_32BIT | GDT_GRAN_EXPND | 0xF)
		},
		/* Kernel data segment */
		(struct gdt_entry){
				.limit_low = 0xFFFF,
				.base_low = 0,
				.base_middle = 0,
				.base_high = 0,
				.gdt_access = (GDT_ACCESS_RW | GDT_ACCESS_P |			\
							   GDT_ACCESS_RING(GDT_PVL_KERNEL) |		\
							   GDT_ACCESS_DT),
				.gdt_gran = (GDT_GRAN_32BIT | GDT_GRAN_EXPND | 0xF)
		},
		/* User code segment */
		(struct gdt_entry){
				.limit_low = 0xFFFF,
				.base_low = 0,
				.base_middle = 0,
				.base_high = 0,
				.gdt_access = (GDT_ACCESS_RW | GDT_ACCESS_EX | GDT_ACCESS_P | \
							   GDT_ACCESS_RING(GDT_PVL_USER) | GDT_ACCESS_DT),
				.gdt_gran = (GDT_GRAN_32BIT | GDT_GRAN_EXPND | 0xF),
		},
		/* User data segment */
		(struct gdt_entry){
				.limit_low = 0xFFFF,
				.base_low = 0,
				.base_middle = 0,
				.base_high = 0,
				.gdt_access = (GDT_ACCESS_RW | GDT_ACCESS_P |			\
							   GDT_ACCESS_RING(GDT_PVL_USER) | GDT_ACCESS_DT),
				.gdt_gran = (GDT_GRAN_32BIT | GDT_GRAN_EXPND | 0xF),
		},
};

struct gdt			_gp = {
		.limit = GDT_NENTRY * sizeof(struct gdt_entry),
		.entp = gdt_entries
};
