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

#include "check_module.h"
#include "log.h"

#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

struct module *modules = NULL;

struct module* new_module(char* filename)
{
  void* handle = dlopen(filename, RTLD_LAZY);
  if (!handle)
    return NULL;
  void* check = dlsym(handle, "check");
  if (!check) {
    dlclose(handle);
    return NULL;
  }
  struct module* output = malloc(sizeof(struct module));
  output->context = NULL;
  init_func *init = dlsym(handle, "init");
  if (init)
    output->context = init();
  output->fail_counter = -1;
  output->mod_handle = handle;
  output->check_func = check;
  output->log_func = dlsym(handle, "log");
  output->next = NULL;
  if (!modules)
    modules = output;
  else {
    struct module* node = modules;
    while (node->next)
      node = node->next;
    node->next = output;
  }
  return output;
};

int check_modules()
{
  struct module* node = modules;
  while (node) {
    int ret = node->check_func(node->context);
    if (ret == -1) {
      if (node->fail_counter != -1) {
        node->fail_counter++;
        if (node->log_func) {
          char buf[BUFSIZ];
          if (node->log_func(buf, sizeof(buf), node->context) > 0)
            write_to_log(buf);
        }
        if (node->fail_counter >= 3)
          return 1;
      }
    } else
      node->fail_counter = 0;
    node = node->next;
  }
  return 0;
};

void reset_modules()
{
  struct module* node = modules;
  while (node) {
    node->fail_counter = -1;
    node = node->next;
  }
};