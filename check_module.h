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

#ifndef _CHECK_MODULE_H
#define _CHECK_MODULE_H

#include <stddef.h>

typedef int check();
typedef int log_msg(char* buf, size_t buflen);

struct module {
  void* mod_handle;
  char fail_counter;
  check* check_func;
  log_msg* log_func;
  struct module* next;
};

struct module* new_module(char* filename);

int check_modules();

void reset_modules();

#endif //_CHECK_MODULE_H