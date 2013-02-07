#include "check_tcp.h"

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
  output->fail_counter = 0;
  output->next = NULL;
  return output;
}

int check_tcp_ports()
{
  struct tcp_port* node = tcp_ports;
  while (node) {
    int sock;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
      perror("socket();");
      exit(1);
    }
    struct sockaddr_in host;
    host.sin_family = AF_INET;
    host.sin_port = htons(node->port);
    host.sin_addr.s_addr = inet_addr("127.0.0.1");
    printf("connect()\n");
    int ret;
    ret = connect(sock, (struct sockaddr *) &host, sizeof(host));
    printf("ret: %d\n", ret);
    close(sock);
    if (ret == -1) {
      node->fail_counter++;
      if (node->fail_counter > 3)
        return 1;
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
    node->fail_counter = 0;
    node = node->next;
  }
}