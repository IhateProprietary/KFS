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

#ifndef __MINIMAL_STRINGOP_H__
# define __MINIMAL_STRINGOP_H__

# include "xstdint.h"

# define is_digit(c) (0x30 <= (c) && 0x39 >= (c))

extern void *_memmove(void *, const void *, size_t);
extern void *_memcpy(void *, const void *, size_t);
extern void *_memchr(const void *, int, size_t);
extern int _memcmp(const void *, const void *, size_t);
extern void *_memset(void *, int, size_t);

extern size_t _strlen(const char *);
extern int _strcmp(const char *, const char *);
extern char *_strcpy(char *, const char *);
extern char *_strchr(const char *, int);

#endif /* __MINIMAL_STRINGOP_H__ */
