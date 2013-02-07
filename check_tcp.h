#ifndef _CHECK_TCP_H
#define _CHECK_TCP_H

struct tcp_port {
  unsigned short port;
  unsigned char fail_counter;
  struct tcp_port *next;
};

struct tcp_port *tcp_ports;

struct tcp_port* new_tcp_port();

int check_tcp_ports();

#endif // _CHECK_TCP_H