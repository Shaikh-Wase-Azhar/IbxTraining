#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#define PORT 8080
#define PORT2 9090

int main(int argc, char const *argv[])
{
	int server_fd, new_client_socket, valread;
	int server_fd2, new_client_socket2, valread2;

	struct sockaddr_in address;
	struct sockaddr_in address2;

	int opt = 1;
	int addrlen = sizeof(address);
	int addrlen2= sizeof(address2);
	
	char buffer[1024] = {0};
	char buffer2[1024] = {0};
	char *hello = "Hello from server...";
	
	// Creating socket file descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	if ((server_fd2 = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}
	
	// Forcefully attaching socket to the port 8080
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
												&opt, sizeof(opt)))
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}

	if (setsockopt(server_fd2, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
												&opt, sizeof(opt)))
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons( PORT );

	address2.sin_family = AF_INET;
	address2.sin_addr.s_addr = INADDR_ANY;
	address2.sin_port = htons( PORT2 );
	
	// Forcefully attaching socket to the port 8080
	if (bind(server_fd, (struct sockaddr *)&address,
								sizeof(address))<0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	if (bind(server_fd2, (struct sockaddr *)&address2,
								sizeof(address2))<0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	if (listen(server_fd, 3) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}

	if (listen(server_fd2, 3) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}

	//while(1) {
		if ((new_client_socket = accept(server_fd, (struct sockaddr *)&address,
						(socklen_t*)&addrlen))<0)
		{
			perror("accept");
			exit(EXIT_FAILURE);
		}
		
		valread = read( new_client_socket , buffer, 1024);
		printf("%s\n",buffer );
		send(new_client_socket , hello , strlen(hello) , 0 );
		printf("Hello message sent to client1\n\n");

		if ((new_client_socket2 = accept(server_fd2, (struct sockaddr *)&address2,
						(socklen_t*)&addrlen2))<0)
		{
			perror("accept");
			exit(EXIT_FAILURE);
		}

		valread2 = read( new_client_socket2 , buffer2, 1024);
		printf("%s\n",buffer2 );
		send(new_client_socket2 , hello , strlen(hello) , 0 );
		printf("Hello message sent to client2\n\n");
		// // closing client connection..!
		
		//for(int ct=0;ct<3;ct++) {
			printf("Enter Message for client1:");
			//printf("Enter %d Message \n",ct);
			char buff[20];
			scanf("%s",buff);
			send(new_client_socket , buff , strlen(buff) , 0 );
			printf("Sending %s to client1\n\n",buff);
			close(new_client_socket);			
		//}

		//for(int ct=0;ct<3;ct++) {

			printf("Enter Message  for client2:");	
			//printf("Enter %d Message \n",ct);
			char buff2[20];
			scanf("%s",buff2);
			send(new_client_socket2 , buff2 , strlen(buff2) , 0 );
			printf("Sending %s to client2\n",buff2);
			close(new_client_socket2);			
		//}
	//}
	close(server_fd); // closing server fd
	close(server_fd2);
	return 0;
}