#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080
#define MAX 80
void transfer(int sock) {
  struct timeval tv;
  tv.tv_sec = 1;
  tv.tv_usec = 0;

  setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO,
   (const char *)&tv, sizeof tv);

  printf("\nSend\tFrame status\tACK status\n");
  int *n, i = 0, j, old, k = 0;
  n = &i;
  for (j = 0; j < 10; j++) {
    old = i;
  LABEL:
    k++;
    i = j % 2;
    write(sock, n, sizeof(i));
    printf("\n%d\t Frame no:%d\t", j, i);
    if (k == 3) {
      printf("ACK missing");
      goto LABEL;
      continue;
    }
    if (read(sock, n, sizeof(i))) {
      if (i == old) {
        printf("Frame missing");
        goto LABEL;
      } else
        printf("ACK no:%d", i);
    }
  }
}

int main(int argc, char const *argv[]) {
  int server_fd, new_socket;
  struct sockaddr_in address;
  int opt = 1, n;
  int addrlen = sizeof(address);

  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }

  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
                 sizeof(opt))) {
    perror("setsockopt");
    exit(EXIT_FAILURE);
  }
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);

  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }
  if (listen(server_fd, 3) < 0) {
    perror("listen");
    exit(EXIT_FAILURE);
  }
  if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                           (socklen_t *)&addrlen)) < 0) {
    perror("accept");
    exit(EXIT_FAILURE);
  }

  transfer(new_socket);

  close(new_socket);
  shutdown(server_fd, SHUT_RDWR);
  return 0;
}
