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

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>

int run = 1;

void stop_listening(int sig) {
  if (run) {
    printf("Receive %d, we're going to stop listening after the next closed connection.\n", sig);
    run = 0;
  }
}

int main(int argc , char **argv) {
  printf("My pid is %d\n", getpid());
  int socket_desc, client_sock, c, read_size;
  struct sockaddr_in server, client;
  char client_message[2000];
  socket_desc = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_desc == -1)
    printf("Could not create socket");
  printf("Socket created\n");
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(8888);
  if (bind(socket_desc,(struct sockaddr *)&server, sizeof(server)) < 0) {
    perror("bind failed. Error");
    return 1;
  }
  printf("Bind done\n");
  listen(socket_desc, 3);
  printf("Waiting for incoming connections...\n");
  signal(SIGINT, stop_listening);
  c = sizeof(struct sockaddr_in);
  while (run) {
    client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
    if (client_sock < 0) {
      perror("accept failed");
      return 1;
    }
    printf("Connection accepted\n");
    while ((read_size = recv(client_sock, client_message, 2000, 0)) > 0)
      write(client_sock, client_message, strlen(client_message));
    if (read_size == 0) {
      printf("Client disconnected\n");
      fflush(stdout);
    } else if(read_size == -1)
      perror("recv failed");
  }
  close(socket_desc);
  printf("Now we just sleep until watchpuppy knows we're done..\n");
  sleep(60);
  return 0;
}