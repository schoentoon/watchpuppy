#include "check_pid.h"
#include "check_tcp.h"

#include <stdio.h>
#include <sys/wait.h>

int check_pid(int pid) {
  int output;
  output = waitpid(pid, NULL, WNOHANG);
  return output;
}