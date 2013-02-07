#include "check_pid.h"
#include "check_tcp.h"

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

static const struct option g_LongOpts[] = {
  { "execute",  required_argument, 0, 'e' },
  { "tcp-port", required_argument, 0, 't' },
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
    while (check_pid(pid) == 0) {
      if (check_tcp_ports()) {
        printf("One of our tcp ports are dead..\n");
      }
      sleep(1);
    }
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
  int iArg, iOptIndex = -1;
  int port;
  while ((iArg = getopt_long(argc, argv, "e:t:", g_LongOpts, &iOptIndex)) != -1) {
    switch (iArg) {
      case 'e':
        execute = optarg;
        break;
      case 't':
        port = atoi(optarg);
        if (port >= 0 && port <= 65535) {
          struct tcp_port* tcp_port = new_tcp_port();
          tcp_port->port = (unsigned short) port;
          if (!tcp_ports)
            tcp_ports = tcp_port;
          else {
            struct tcp_port* node = tcp_ports;
            while (node->next)
              node = node->next;
            node->next = tcp_port;
          }
        }
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