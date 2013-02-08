#include "check_pid.h"
#include "check_tcp.h"

#include <stdio.h>
#include <sys/wait.h>

int check_pid(int pid) {
  return waitpid(pid, NULL, WNOHANG);
}