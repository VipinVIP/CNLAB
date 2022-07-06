#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

#define PORT 8080
#define window_size 5

void transfer(int sock) {
  struct timeval tv;
  tv.tv_sec = 1;
  tv.tv_usec = 0;
  setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char *)&tv, sizeof tv);
  srand(time(NULL));
  int *n, head = 0, tail = 0, c = 0, k = 0, count, red;
  red = head;
  n = &red;
  printf("\nEnter number of frames : ");
  scanf("%d", &count);
  printf("\n");
  for (int i = 0; i < count; i++) {
    red = -1;
    if ((tail - head) > window_size || tail == count - 1) {
      printf("\nTimeout resend from %d\n\n", head);
      tail = head;
      i = head;
    }

    int flag = rand() % 2;
    printf("Frame %d send\n", tail);
    if (flag) {
      write(sock, &tail, sizeof(tail));
    }
    tail++;

    read(sock, &red, sizeof(red));
    if (red != -1) {
      head++;
      printf("ACK %d\n", red);
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
