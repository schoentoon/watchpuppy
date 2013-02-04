#include "check_pid.h"

#include <stdio.h>
#include <sys/wait.h>

int check_pid(int pid) {
  int status;
  int wPid = waitpid(pid, &status, WNOHANG);
  printf("status: %d, waitpid: %d\n", status, wPid);
  return wPid;
}