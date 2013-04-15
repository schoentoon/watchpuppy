/*  watchpuppy
 *  Copyright (C) 2013  Toon Schoenmakers
 *
 *  This program is free software: you can redistribute it and/or modify
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
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _DEBUG_H
#define _DEBUG_H

/** Compile using -DDEV ($ make dev) to enable the
 * DEBUG(...) macro, which works exactly the same
 * as printf. In case it's compiled without -DDEV
 * it won't even compile these statements in.
 */
#ifdef DEV
#  include <stdio.h>
#  define DEBUG(...) printf(__VA_ARGS__);
#else
#  define DEBUG(...)
#endif

#endif //_DEBUG_H