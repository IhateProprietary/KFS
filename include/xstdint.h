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

typedef __UINT_LEAST32_TYPE__ u32;
typedef __UINT_LEAST16_TYPE__ u16;
typedef __UINT_LEAST8_TYPE__ u8;

typedef __INT_LEAST32_TYPE__ s32;
typedef __INT_LEAST16_TYPE__ s16;
typedef __INT_LEAST8_TYPE__ s8;

# if __x86_64__
typedef __UINT_LEAST64_TYPE__ u64;
typedef __INT_LEAST64_TYPE__ s64;
typedef u64 size_t;
typedef s64 ssize_t;
#  define __SIZEOF_SIZE_T__ 8
#  define __SIZE_T_WIDTH__ __UINT_LEAST64_WIDTH__
#  define __SIZE_T_MAX__ __UINT_LEAST64_MAX__
# else
typedef u32 size_t;
typedef s32 ssize_t;
#  define __SIZEOF_SIZE_T__ 4
#  define __SIZE_T_WIDTH__ __UINT_LEAST32_WIDTH__
#  define __SIZE_T_MAX__ __UINT_LEAST32_MAX__
# endif

#endif
