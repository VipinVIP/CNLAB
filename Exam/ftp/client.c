#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>

void write_file(FILE *file,int sockfd){
    int n;
    char data[1024]={0};

    while(1){
        n=recv(sockfd,data,sizeof(data),0);
        if(n<=0){
            break;
            return ;
        }
        fprintf(file,"%s",data);
        memset(data,'0',sizeof(data));
    }
}

int main()
{
    struct sockaddr_in address;
    int socketDes , result;
    char buffer[10];
    FILE *file;
    
    memset(&address,'0',sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    address.sin_port = htons(8080);
    
	socketDes = socket(AF_INET,SOCK_STREAM,0);

	result = connect(socketDes,(struct sockaddr*)&address,sizeof(address));

	printf("\nEnter The Name of the File :  ");
	scanf("%s",buffer);
	send(socketDes,buffer,sizeof(buffer),0);

    file=fopen(buffer,"w");
    write_file(file,  socketDes);

    close(socketDes);
}