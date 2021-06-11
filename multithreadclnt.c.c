//UDP Client											

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 8010

//Structure to send
struct main
{
    char file[20];
    int command;
};
struct main data;

int main() {
	int sockfd;
	char buffer[1024];
	//char hello[1024];
	struct sockaddr_in	servaddr;
	// Creating socket
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}

	memset(&servaddr, 0, sizeof(servaddr));
	
	// Filling server information
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT);
	servaddr.sin_addr.s_addr = INADDR_ANY;
	while(1)
	{
		int n, len;
		printf("Enter command 1)Read\n");
	    scanf("%d",&data.command);
    	printf("Enter the File name (Gnany,Abhi,Nani):\n");
    	scanf("%s",data.file);

		sendto(sockfd, (const char *)&data, sizeof(data),MSG_CONFIRM,(const struct sockaddr *) &servaddr,sizeof(servaddr));
		printf("Structure sent.\n");
		
		//Receive 1 line of data from File
		n = recvfrom(sockfd,(char *)buffer,1024,MSG_WAITALL,(struct sockaddr *) &servaddr,&len);
		buffer[n] = '\0';
		printf("Server : %s\n", buffer);
	}
	close(sockfd);
	return 0;
}
