#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

static const struct option g_LongOpts[] = {
  { "execute", required_argument, 0, 'e' },
  { 0, 0, 0, 0 }
};

int main(int argc, char** argv) {
  char* execute = NULL;
  int iArg, iOptIndex = -1;
  while ((iArg = getopt_long(argc, argv, "e:", g_LongOpts, &iOptIndex)) != -1) {
    switch (iArg) {
      case 'e':
        execute = optarg;
        break;
    }
  }
  if (!execute) {
    printf("Missing the -e flag.\n");
    return 1;
  }
  while (1) {
    int ret = system(execute);
    if (WIFSIGNALED(ret) && (WTERMSIG(ret) == SIGINT || WTERMSIG(ret) == SIGQUIT)) {
      printf("User cancelled the process.\n");
      return 0;
    }
    printf("return: %i\n", ret);
  }
  return 0;
}