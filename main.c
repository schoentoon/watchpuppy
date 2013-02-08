#include "check_pid.h"
#include "check_tcp.h"

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>
#include <limits.h>

static int interval = 10;

static const struct option g_LongOpts[] = {
  { "execute",  required_argument, 0, 'e' },
  { "tcp-port", required_argument, 0, 't' },
  { "interval", required_argument, 0, 'i' },
  { 0, 0, 0, 0 }
};

int start(char* command) {
  int pid = fork();
  if (pid == -1) {
    printf("Error: %s", strerror(errno));
    exit(2);
  }
  if (pid > 0) {
    printf("pid: %d\n", pid);
    while (1) {
      if (check_pid(pid))
        break;
      if (check_tcp_ports()) {
        printf("One of our tcp ports are dead.. Let's send it a SIGKILL.\n");
        kill(pid, 9);
        break;
      }
      sleep(interval);
    }
    fprintf(stderr, "Our child seems dead :( let's restart it.\n");
    reset_tcp_ports();
    start(command);
    return 0;
  } else if (pid == 0) {
    putenv("MALLOC_CHECK_=3");
    const char *argv[] = { "sh", "-c", command, NULL };
    execvp("sh", (char * const *) argv);
    printf("execvp failed\n");
    exit(2);
  } else {
    printf("fork returned: %d\n", pid);
    exit(3);
  }
}

int main(int argc, char** argv) {
  char* execute = NULL;
  int iArg, iOptIndex, tmp = -1;
  while ((iArg = getopt_long(argc, argv, "e:i:t:", g_LongOpts, &iOptIndex)) != -1) {
    switch (iArg) {
      case 'e':
        execute = optarg;
        break;
      case 't':
        tmp = strtol(optarg, NULL, 10);
        if ((errno == ERANGE || (tmp == LONG_MAX || tmp == LONG_MIN)) || (errno != 0 && tmp == 0) || tmp < 0 || tmp > 65535) {
          fprintf(stderr, "--tcp-port requires a valid port to check.");
          return 1;
        }
        struct tcp_port* tcp_port = new_tcp_port();
        tcp_port->port = (unsigned short) tmp;
        if (!tcp_ports)
          tcp_ports = tcp_port;
        else {
          struct tcp_port* node = tcp_ports;
          while (node->next)
            node = node->next;
          node->next = tcp_port;
        }
        break;
      case 'i':
        tmp = strtol(optarg, NULL, 10);
        if ((errno == ERANGE || (tmp == LONG_MAX || tmp == LONG_MIN)) || (errno != 0 && tmp == 0) || tmp < 0) {
          fprintf(stderr, "--interval requires a positive amount of seconds.\n");
          return 1;
        }
        interval = tmp;
        break;
    }
  }
  if (!execute) {
    fprintf(stderr, "Missing the -e flag.\n");
    return 1;
  }
  start(execute);
  return 0;
}