
#include <arpa/inet.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080

void transfer(int sock) {

  int *n, i, e = 0, c = 0, k = 0;
  n = &i;

  while (read(sock, n, sizeof(i)) != 0) {
    if (e == i) {
      printf("\nFrame %d recived\t", e);
      e++;
      i++;
      write(sock, n, sizeof(i));
      printf("\nAck %d\n", i);
    }
  }
}

int main(int argc, char const *argv[]) {
  int sock = 0, valread, client_fd, n;
  struct sockaddr_in serv_addr;
  char str[5];
  char buffer[1024] = {0};
  
  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    printf("\nSocket creation error \n");
    return -1;
  }

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);

  if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
    printf("\nInvalid address/ Address not supported \n");
    return -1;
  }

  if ((client_fd = connect(sock, (struct sockaddr *)&serv_addr,
                           sizeof(serv_addr))) < 0) {
    printf("\nConnection Failed \n");
    return -1;
  }

  transfer(sock);

  close(client_fd);
  return 0;
}
