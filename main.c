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
#include "check_pid.h"
#include "check_tcp.h"
#include "debug.h"
#include "hook.h"
#include "log.h"

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>
#include <limits.h>
#include <time.h>
#include <dlfcn.h>

static int interval = 10;
static int minimum_crash_time = 60;
int program_pid = 0;

static const struct option g_LongOpts[] = {
  { "execute",  required_argument, 0, 'e' },
  { "tcp-port", required_argument, 0, 't' },
  { "interval", required_argument, 0, 'i' },
  { "help",     no_argument,       0, 'h' },
  { "hook",     required_argument, 0, 'H' },
  { "log",      required_argument, 0, 'l' },
  { "minimum",  required_argument, 0, 'm' },
  { "module",   required_argument, 0, 'M' },
  { 0, 0, 0, 0 }
};

void quit(int signal) {
  if (program_pid > 0) {
    DEBUG("We've reiceved a %d, so we're telling our child to stop before we stop.\n", signal);
    kill(program_pid, SIGINT); /* First we'll tell our child to exit */
    sleep(interval);           /* Kids are slow, let's give it some time.. */
    kill(program_pid, SIGKILL);/* Still not dead? Fine, let's kill it with fire. */
  }
  exit(0);
}

unsigned int start(char* command) {
  int pid = fork();
  if (pid == -1) {
    fprintf(stderr, "Error: %s\n", strerror(errno));
    exit(2);
  }
  if (pid > 0) {
    program_pid = pid;
    DEBUG("pid: %d\n", pid);
    write_to_log("Started '%s' with pid %d", command, pid);
    unsigned int begin = time(NULL);
    while (1) { /* We'll break out of this loops once we seem dead. */
      if (check_pid(pid))
        break;
      if (check_tcp_ports()) {
        kill(pid, SIGKILL);
        break;
      }
      if (check_modules()) {
        kill(pid, SIGKILL);
        break;
      }
      sleep(interval);
    }
    fprintf(stderr, "Our child seems dead :( let's restart it.\n");
    reset_tcp_ports();
    execute_hooks();
    write_to_log("Our child '%s' died", command);
    return (unsigned int) time(NULL) - begin;
  } else if (pid == 0) {
    putenv("MALLOC_CHECK_=3");
    const char *argv[] = { "sh", "-c", command, NULL };
    execvp("sh", (char * const *) argv);
    fprintf(stderr, "execvp failed\n");
    exit(2);
  } else {
    write_to_log("fork() returned %d, this is bad.", pid);
    DEBUG("fork() returned %d, this is bad.\n", pid);
    exit(3);
  }
}

void usage() {
  printf("USAGE: watchpuppy [options]\n");
  printf("Options are:\n");
  printf(" -e, --execute [program]\tProgram to execute, use quotes if you want to pass arguments.\n");
  printf(" -t, --tcp-port [port]\t\tPort to monitor, once it won't be able to connect it'll restart your program.\n");
  printf(" -i, --interval [seconds]\tAmount of seconds between checks if it's still alive, defaults to 10 seconds.\n");
  printf(" -l, --log [logfile]\t\tWrite a log to this file.\n");
  printf(" -H, --hook [executable]\tProgram to execute in case we have to restart our child (think email scripts).\n");
  printf(" -m, --minimum [seconds]\tMinimum amount of seconds of runtime required for a restart, defaults to 60 seconds.\n");
  printf("\t\t\t\tIn case the runtime of your executable is in fact done 'too quickly' watchpuppy will silently exit.\n");
  printf("\t\t\t\tYou can set this option to 0 to disable this check.\n");
  printf(" -M, --module [module path]\tUse this module to perform checks if we're still running.\n");
  printf(" -h, --help\t\t\tShow this help page.\n");
}

int check_access(char* command) {
  char* tmp = strdup(command);
  int output = access(strtok(tmp, " "), F_OK|X_OK);
  free(tmp);
  return output;
}

int main(int argc, char** argv) {
  char* execute = NULL;
  int iArg, iOptIndex = -1;
  long tmp = -1;
  while ((iArg = getopt_long(argc, argv, "e:i:t:hH:l:m:M:", g_LongOpts, &iOptIndex)) != -1) {
    switch (iArg) {
      case 'e':
        execute = optarg;
        break;
      case 't':
        tmp = strtol(optarg, NULL, 10);
        if ((errno == ERANGE || (tmp == LONG_MAX || tmp == LONG_MIN)) || (errno != 0 && tmp == 0) || tmp < 0 || tmp > 65535) {
          fprintf(stderr, "--tcp-port requires a valid port to check.\n");
          return 1;
        }
        struct tcp_port* tcp_port = new_tcp_port();
        tcp_port->port = (unsigned short) tmp;
        break;
      case 'M': {
        struct module* module = new_module(optarg);
        if (!module) {
          fprintf(stderr, "There was an error loading '%s', %s.\n", optarg, dlerror());
        }
        break;
      }
      case 'H':
        if (check_access(optarg)) {
          fprintf(stderr, "The file passed to --hook either doesn't exist or isn't executable.\n");
          return 1;
        }
        struct hook* hook = new_hook();
        hook->executable = optarg;
        break;
      case 'i':
        tmp = strtol(optarg, NULL, 10);
        if ((errno == ERANGE || (tmp == LONG_MAX || tmp == LONG_MIN)) || (errno != 0 && tmp == 0) || tmp < 0) {
          fprintf(stderr, "--interval requires a positive amount of seconds.\n");
          return 1;
        }
        interval = tmp;
        break;
      case 'm':
        tmp = strtol(optarg, NULL, 10);
        if ((errno == ERANGE || (tmp == LONG_MAX || tmp == LONG_MIN)) || (errno != 0 && tmp == 0) || tmp < 0) {
          fprintf(stderr, "--minimum requires a positive amount of seconds.\n");
          return 1;
        }
        minimum_crash_time = tmp;
        break;
      case 'l':
        if (strcmp(optarg, SYSLOG) == 0)
          logfile = SYSLOG;
        else
          logfile = optarg;
        break;
      default:
      case 'h':
        usage();
        return 0;
    }
  }
  if (!execute) {
    fprintf(stderr, "Missing the -e flag.\n");
    return 1;
  } else if (check_access(execute)) {
    fprintf(stderr, "%s either doesn't exist or isn't executable.\n", execute);
    return 1;
  }
  signal(SIGINT, quit);
  signal(SIGQUIT, quit);
  unsigned int restarts = 0;
  for (;;restarts++) {
    unsigned int runtime = start(execute);
    DEBUG("Runtime: %d\n", runtime);
    if (runtime < minimum_crash_time) {
      write_to_log("Runtime is less than the minimum runtime specified, so we're quiting.");
      return 1;
    }
  }
  return 0;
}
