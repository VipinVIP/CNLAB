#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>

int main()
{
    struct sockaddr_in address;
    int socketDes , result,choix,SEND_SIZE=64000,bytesReceived=0,FileLenght;
    char buffer[10],message[2048],*fileTemp;
    FILE *file;
    
    memset(&address,'0',sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    address.sin_port = htons(8080);
    
	socketDes = socket(AF_INET,SOCK_STREAM,0);

	result = connect(socketDes,(struct sockaddr*)&address,sizeof(address));

    while(1)
    {
    	printf("\n	Choose one of the available options :  \n\n");
    	printf("1 - display The Contents of the server  .\n");
    	printf("2 - download a File from The Server . \n");
    	printf("3 - Quit .\n");
    	printf("\n> ");
    	scanf("%d",&choix);
    	switch(choix)
    	{
    		case 1 : 
    			strcpy(buffer,"1");
    			send(socketDes,buffer,sizeof(buffer),0);

				printf("\nthe Content of the working folder in the server is : \n");
		    	result = read(socketDes,message,sizeof(message));
		    
		    	printf("%s\n",message);		
    			break;

    		case 2 : 
    			strcpy(buffer,"2");
				send(socketDes,buffer,sizeof(buffer),0);

    			printf("\nEnter The Name of the File :  ");
    			scanf("%s",message);

    			send(socketDes,message,sizeof(message),0);
		    	
		    	read(socketDes,buffer,sizeof(buffer));
		    	
		    	FileLenght = atoi(buffer);
		    	
		    	fileTemp = (char*)malloc(SEND_SIZE*sizeof(char));

		    	if((file = fopen(message,"w+")) == NULL)
		    	{
		    		fprintf(stderr,"Failed to Open The File  ... \n");
			    	exit(1);
		    	}
		    	
		    	while(1)
		    	{	
		    		if(FileLenght - bytesReceived < SEND_SIZE)
		    		{
		    			SEND_SIZE = FileLenght - bytesReceived;
		    			fileTemp = realloc(fileTemp,SEND_SIZE);
		    		}
		    		    	
		    		if((result = read(socketDes,fileTemp,sizeof(fileTemp))) == -1)
		    		{
		    			fprintf(stderr,"Failed to received The File  ... \n");
			    		exit(1);
		    		}
		    		
		    		bytesReceived += result;
		    		
		    		if((result = fwrite(fileTemp,1,sizeof(fileTemp),file)) == -1)
		    		{
		    			fprintf(stderr,"Failed to write in The File  ... \n");
			    		exit(1);
		    		}
		    		
		    		if(FileLenght == bytesReceived)
		    		{
		    			printf("\nFile Received Successfully ... \n");
		    			printf("%d bytes received , %d File Lenght \n",bytesReceived,FileLenght);
		    			fclose(file);
		    			break;
		    		}
		    		
		    	}
    			break; 
				
    		case 3:
    			return 0;
    			break;

    		default : printf("Please Choose One of The available Options ... \n");
    	}
    }

return 0;
}

