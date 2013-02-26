#include "hook.h"

#include "log.h"
#include "debug.h"

#include <stdlib.h>
#include <unistd.h>

struct hook *hooks = NULL;

struct hook* new_hook()
{
  struct hook* output = malloc(sizeof(struct hook));
  output->next = NULL;
  if (!hooks)
    hooks = output;
  else {
    struct hook* node = hooks;
    while (node->next)
      node = node->next;
    node->next = output;
  }
  return output;
}

void execute_hooks()
{
  if (hooks) {
    DEBUG("Forking to execute our hooks.\n");
    if (fork() == 0) {
      struct hook* node = hooks;
      while (node) {
        DEBUG("Executing %s\n", node->executable);
        int output = system(node->executable);
        if (output != 0)
          write_to_log("There was an error while executing hook '%s'", node->executable);
        node = node->next;
      }
      exit(0);
    }
  }
}
