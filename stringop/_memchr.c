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

#include "private_stringop.h"

static inline void *_memchr8(OP dstp, int c, size_t n)
{
		register u8	dstc;

		while (n--) {
				dstc = ((u8 *)dstp)[0];
				if (dstc == c)
						return ((void *)dstp);
				++dstp;
		}
		return (0);
}

static void *_memchr64(OP dstp, int c, size_t n)
{
		register size_t	xlen;
		register OP		mask_set;
		register OP		lo_magic;
		register OP		hi_magic;
		OP				dstpp;

		lo_magic = LBITS;
		hi_magic = HBITS;
		mask_set = c << 8 | c;
		mask_set = (mask_set << 16) | mask_set;
		xlen = n >> OP_SHIFT;
		while (xlen--) {
				if ((((((OP *)dstp)[0] ^ mask_set) - lo_magic) & hi_magic))
						if ((dstpp = (OP)_memchr8(dstp, c, OP_SIZE)))
								return ((void *)dstpp);
				dstp += OP_SIZE;
		}
		return (_memchr8(dstp, c, n & OP_MASK));
}

void *_memchr(const void *mem, int c, size_t n)
{
		OP		dstp;
		size_t	xlen;
		void	*ret;

		dstp = (OP)mem;
		c &= 0xff;
		if (n >= (OP_SIZE * 2)) {
				xlen = -dstp & OP_MASK;
				if ((ret = _memchr8(dstp, c, xlen)))
						return (ret);
				dstp += xlen;
				n -= xlen;
				return (_memchr64(dstp, c, n));
		}
		return (_memchr8(dstp, c, n));
}
