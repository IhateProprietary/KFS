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

#ifndef __MSR_H__
# define __MSR_H__ 1

# define rdmsr(reg, high, low)				\
	__asm__ __volatile__("rdmsr"			\
			     :"=a"(low), "=d"(high)	\
			     :"c"(reg))

# define wrmsr(reg, high, low)					\
	__asm__ volatile("wrmsr"				\
			 ::"a"(low), "d"(high), "c"(reg))


#endif /* __MSR_H__ */
