#ifndef _HOOK_H
#define _HOOK_H

struct hook {
  char* executable;
  struct hook *next;
};

/** Used to create a new hook struct
 * This will automatically append it to the
 * list that is used by execute_hooks()
 */
struct hook* new_hook();

/** This will loop through the list of hook
 * structs and will try to execute them. We'll
 * totally ignore any error output from any program.
 */
void execute_hooks();

#endif //_HOOK_H