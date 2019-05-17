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

#ifndef __SIMPLE_SPIN_LOCK_H__
# define __SIMPLE_SPIN_LOCK_H__
# include <stdatomic.h>

typedef atomic_uint_fast32_t _spin_lock_t;

extern void simple_spin_lock(_spin_lock_t *lock);
extern void simple_spin_trylock(_spin_lock_t *lock);
extern void simple_spin_unlock(_spin_lock_t *lock);

#endif /* __SIMPLE_SPIN_LOCK_H__ */
