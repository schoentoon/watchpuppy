#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>

static const struct option g_LongOpts[] = {
  { "execute", required_argument, 0, 'e' },
  { 0, 0, 0, 0 }
};

int start(char* command) {
  int pid = fork();
  if (pid == -1) {
    printf("Error: %s", strerror(errno));
    exit(2);
  }
  if (pid > 0) {
    printf("Pid: %d\n", pid);
    waitpid(pid, NULL, 0);
    start(command);
    return 0;
  }
  char* args[sizeof(command)] = {command};
  execvp(command, args);
  printf("execvp failed\n");
  exit(2);
}

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
  start(execute);
  return 0;
}