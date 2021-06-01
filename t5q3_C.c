// Server side implementation of UDP client-server model
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define UDPPORT	 1090

int main() 
{
	char buffer[100];
    int sockfd;
	struct sockaddr_in servaddr,cliaddr;
	  //char *hello = "Hello from client";
	
	// Creating socket file descriptor
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}
	memset(&servaddr, 0, sizeof(servaddr));
	
	
	// Filling server information
	servaddr.sin_family = AF_INET; // IPv4
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(UDPPORT);
	
	int len, n;

	len = sizeof(cliaddr); //len is value/result
	//sendto(sockfd, (const char *)hello, strlen(hello),MSG_CONFIRM, (const struct sockaddr *) &servaddr,sizeof(servaddr));
	n = recvfrom(sockfd, (char *)buffer,1024,MSG_WAITALL, (struct sockaddr *) &servaddr,&len);
	buffer[n] = '\0';
	printf("Client : %s\n", buffer);
	close(sockfd);
	return 0;
}