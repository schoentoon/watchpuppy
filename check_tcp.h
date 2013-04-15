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