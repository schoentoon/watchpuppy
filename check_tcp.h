#ifndef _CHECK_TCP_H
#define _CHECK_TCP_H

struct tcp_port {
  unsigned short port;
  char fail_counter;
  struct tcp_port *next;
};

/** Used to create a new tcp_port struct
 * This will automatically append it to the
 * list that is used by check_tcp_ports() and
 * reset_tcp_ports()
 */
struct tcp_port* new_tcp_port();

/** This will loop through the list of tcp_port
 * structs and will try to connect to them. This
 * function will return 1 in case it couldn't connect
 * to one tcp_port for at least 3 times. That is if
 * it managed to connected earlier.
 */
int check_tcp_ports();

/** This will reset the fail_counter of all the
 * tcp_port structs to their initial state.
 */
void reset_tcp_ports();

#endif // _CHECK_TCP_H