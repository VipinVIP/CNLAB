#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>

#define PORT 8081
#define MAXLINE 1024

void main()  {
	int sockfd;
	char buff[MAXLINE];
	char* dt = "Date and time";
	struct sockaddr_in servaddr;
	
	if((sockfd = socket(AF_INET, SOCK_DGRAM,0)) < 0)  {
		perror("\nSocket creation failed");
		exit(0);
	}
	
	memset(&servaddr,0,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT);
	servaddr.sin_addr.s_addr = INADDR_ANY;
	
	int n, len;
	sendto(sockfd, (const char*)dt, strlen(dt), MSG_CONFIRM, (const struct sockaddr*)&servaddr, sizeof(servaddr));
	printf("\nRequesting time\n");
	n = recvfrom(sockfd, (char*)buff, MAXLINE, MSG_WAITALL, (struct sockaddr*)&servaddr, &len);
	
	buff[n] = '\0';
	printf("\nServer time: %s\n", buff);
	close(sockfd);
}
