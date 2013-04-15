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

#include "check_tcp.h"
#include "debug.h"
#include "log.h"

#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

struct tcp_port *tcp_ports = NULL;

struct tcp_port* new_tcp_port()
{
  struct tcp_port* output = malloc(sizeof(struct tcp_port));
  output->fail_counter = -1;
  output->next = NULL;
  if (!tcp_ports)
    tcp_ports = output;
  else {
    struct tcp_port* node = tcp_ports;
    while (node->next)
      node = node->next;
    node->next = output;
  }
  return output;
}

int check_tcp_ports()
{
  struct tcp_port* node = tcp_ports;
  while (node) {
    int sock;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
      fprintf(stderr, "Couldn't create socket.");
      exit(1);
    }
    struct sockaddr_in host;
    host.sin_family = AF_INET;
    host.sin_port = htons(node->port);
    host.sin_addr.s_addr = inet_addr("127.0.0.1");
    DEBUG("connect()\n");
    int ret;
    ret = connect(sock, (struct sockaddr *) &host, sizeof(host));
    DEBUG("ret: %d\n", ret);
    close(sock);
    if (ret == -1) {
      if (node->fail_counter != -1) {
        node->fail_counter++;
        write_to_log("Port %d seems unreachable, fail counter is at %d.", node->port, node->fail_counter);
        if (node->fail_counter >= 3)
          return 1;
      }
    } else
      node->fail_counter = 0;
    node = node->next;
  }
  return 0;
}

void reset_tcp_ports()
{
  struct tcp_port* node = tcp_ports;
  while (node) {
    node->fail_counter = -1;
    node = node->next;
  }
}