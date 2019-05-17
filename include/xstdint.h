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

#ifndef __XSTDINT_H__
# define __XSTDINT_H__

typedef __UINT_FAST32_TYPE__ u32;
typedef __UINT_FAST16_TYPE__ u16;
typedef __UINT_FAST8_TYPE__ u8;

typedef __INT_FAST32_TYPE__ i32;
typedef __INT_FAST16_TYPE__ i16;
typedef __INT_FAST8_TYPE__ i8;

# if __x86_64__
typedef __UINT_FAST64_TYPE__ u64;
typedef __INT_FAST64_TYPE__ i64;
typedef u64 size_t;
# else
typedef u32 size_t;
# endif

#endif
