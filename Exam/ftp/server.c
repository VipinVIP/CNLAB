#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

void send_file(FILE *file , int sockfd){
    int n;
    char data[1024]={0};

    while(fgets(data,1024,file)!=NULL){
        if(send(sockfd,data,sizeof(data),0)==-1){
            printf("Error sending file");
            exit(1);
        }
        memset(data,'0',sizeof(data));
    }
}

int main(){

  struct sockaddr_in address;
  int socketDes, clientDes, result = 0;
  int socketLenght, FileLenght, bytesSended = 0, SEND_SIZE = 64000;
  char buffer[10], fileName[20], temp[30], *fileTemp;
  FILE *file;

  memset(&address, '0', sizeof(address));

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = htonl(INADDR_ANY);
  address.sin_port = htons(8080);

  socketDes=socket(AF_INET, SOCK_STREAM, 0);

  result=bind(socketDes, (struct sockaddr *)&address, sizeof(address));

  listen(socketDes, 5);

  socketLenght = sizeof(address);

  clientDes = accept(socketDes, (struct sockaddr *)&address,
                     (socklen_t *)&socketLenght);


  read(clientDes, buffer, sizeof(buffer));
  file=fopen(buffer, "r");
  send_file(file,clientDes);

  close(clientDes);
  close(socketDes);

}
