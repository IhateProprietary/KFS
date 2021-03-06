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

#ifndef __KERNEL_H__
# define __KERNEL_H__

# include "xstdint.h"

struct _sysinfo
{
	u32 acpi_rsdp;
	u32 frameinfo;
	u32 elfsection;
	u32 mmap;
	u32 meminfo;
	u32 lapic_addr;
	u32 ioapic_addr;
};

int	printk(const char *fmt, ...);

#endif /* __KERNEL_H__ */
