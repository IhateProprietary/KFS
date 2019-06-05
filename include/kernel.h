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

struct _sysinfo
{
	u32 _acpi_rsdp;
	u32 _frameinfo;
	u32 _elfsection;
	u32 _mmap;
	u32 _meminfo;
} __attribute__((packed));

int	printk(const char *fmt, ...);

#endif /* __KERNEL_H__ */
