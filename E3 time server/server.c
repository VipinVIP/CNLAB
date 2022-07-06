#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<time.h>

#define PORT 8081
#define MAXLINE 1024

void main()  {
	int sockfd;
	char buff[MAXLINE];
	time_t tm;
	time(&tm);
	char* currtime = ctime(&tm);
	struct sockaddr_in servaddr, cliaddr;
	
	if((sockfd = socket(AF_INET, SOCK_DGRAM,0)) < 0)  {
		perror("\nSocket creation failed");
		exit(0);
	}
	
	memset(&servaddr,0,sizeof(servaddr));
	memset(&cliaddr,0,sizeof(cliaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT);
	servaddr.sin_addr.s_addr = INADDR_ANY;
	
	if(bind(sockfd, (const struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)  {
		perror("Socket binding failed");
		exit(0);
	}
	
	int n, len;
	len = sizeof(cliaddr);
	n = recvfrom(sockfd, (char*)buff, MAXLINE, MSG_WAITALL, (struct sockaddr*)&cliaddr, &len);
	buff[n] = '\0';
	printf("\nClient: %s requested\n", buff);
	
	sendto(sockfd, (const char*)currtime, strlen(currtime), MSG_CONFIRM, (const struct sockaddr*)&cliaddr, len);
	printf("\nDate and Time send.\n");
}
