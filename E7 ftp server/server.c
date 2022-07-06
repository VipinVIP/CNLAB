#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char **argv) {
  struct sockaddr_in address;
  int socketDes, clientDes, result = 0;
  int socketLenght, FileLenght, bytesSended = 0, SEND_SIZE = 64000;
  char buffer[10], fileName[20], temp[30], *fileTemp;
  FILE *file;

  memset(&address, '0', sizeof(address));

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = htonl(INADDR_ANY);
  address.sin_port = htons(8080);

  socketDes = socket(AF_INET, SOCK_STREAM, 0);

  result = bind(socketDes, (struct sockaddr *)&address, sizeof(address));

  listen(socketDes, 5);

  socketLenght = sizeof(address);

  clientDes = accept(socketDes, (struct sockaddr *)&address,
                     (socklen_t *)&socketLenght);

  while (1) {
    read(clientDes, buffer, sizeof(buffer));

    switch (atoi(buffer)) {
    case 1:
      dup2(clientDes, STDOUT_FILENO);
      system("ls .");
      break;

    case 2:
      read(clientDes, fileName, sizeof(fileName));

      file = fopen(fileName, "r");

      fseek(file, 0, SEEK_END);
      FileLenght = ftell(file);
      fseek(file, 0, SEEK_SET);

      sprintf(buffer, "%d", FileLenght);
      result = send(clientDes, buffer, sizeof(buffer), 0);

      fileTemp = (char *)malloc(SEND_SIZE * sizeof(char));

      while (1) {
        if (FileLenght - bytesSended < SEND_SIZE) {
          SEND_SIZE = FileLenght - bytesSended;
          fileTemp = realloc(fileTemp, SEND_SIZE);
        }

        if ((result = fread(fileTemp, 1, SEND_SIZE, file)) == -1) {
          fprintf(stderr, "Failed to read from The File ... \n");
          exit(1);
        }

        if ((result = send(clientDes, fileTemp, SEND_SIZE, 0)) == -1) {
          fprintf(stderr, "Failed to send The File ... \n");
          exit(1);
        }

        bytesSended += result;
        if (FileLenght == bytesSended) {
          SEND_SIZE = 64000;
          printf("bytes sended : %d , File Lenght : %d \n", bytesSended,
                 FileLenght);
          fclose(file);
          printf("File Sended successfully ... \n");
          break;
        }
      }

      break;
    }
  }
  close(clientDes);
  close(socketDes);
  return 0;
};
