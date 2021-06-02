#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/types.h>
#include <arpa/inet.h>

#define PORT 8080
#define UDPPORT	9090
#define MAXLINE 1024

int main(int argc, char const *argv[])
{
	int server_fd, new_client_socket, valread;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	
	int sockfd;
	char buffer[MAXLINE];
	char *hello = "Hello from B:..";

	struct sockaddr_in servaddr, cliaddr;

	// Creating socket file descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	

    // Creating socket file descriptor
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		perror("socket creation failed");
		return -1;
	}

	memset(&servaddr, 0, sizeof(servaddr));
	memset(&cliaddr, 0, sizeof(cliaddr));
	
	// Filling server information
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(UDPPORT);
	servaddr.sin_addr.s_addr = INADDR_ANY;
	
	

	// Forcefully attaching socket to the port 8080
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,&opt, sizeof(opt)))
												
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons( PORT );
	
	// Forcefully attaching socket to the port 8080
	if (bind(server_fd, (struct sockaddr *)&address,sizeof(address))!=0)
								
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}


		// Bind the socket with the server address
	if ( bind(sockfd, (const struct sockaddr *)&servaddr,
			sizeof(servaddr)) < 0 )
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}



	if (listen(server_fd, 3) != 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}
	while(1) 
	{
		if ((new_client_socket = accept(server_fd, (struct sockaddr *)&address,(socklen_t*)&addrlen))<0)
		{
			perror("accept");
			exit(EXIT_FAILURE);
		}
		valread = read( new_client_socket , buffer, 100);
		printf("%s\n",buffer );

		

		int len, n;
    	len = sizeof(servaddr);  //len is value/resuslt
        sendto(sockfd, (const char *)buffer, sizeof(buffer) ,MSG_CONFIRM, (const struct sockaddr *)&servaddr,len);
	    printf("Hello message sent.\n");
	
	//	close(new_client_socket);// // closing client connection..!
	}
	close(server_fd); // closing server fd
	close(sockfd);
    return 0;
}

