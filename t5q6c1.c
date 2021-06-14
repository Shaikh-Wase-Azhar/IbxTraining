#include <stdio.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define PORT 8080
#define MAXSIZE 1024
//#define INADDR_ANY  "127.0.0.1"

int main(int argc, char const *argv[])
{
	int clientSocket = 0, valread;
	struct sockaddr_in serverAddr;
	char buffer[MAXSIZE] = {0};
	char filename[10]= {0};
	//char *hellocheck = "Hellocheck";

	if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("\n Socket creation error \n");
		return -1;
	}

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	//memory set to zero
	memset(&serverAddr, 0, sizeof(serverAddr));

	// Convert IPv4 and IPv6 addresses from text to binary form
	if(inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr)<=0)
	{
		printf("\nInvalid address/ Address not supported \n");
		return -1;
	}

	if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
	{
		printf("\nConnection Failed \n");
		return -1;
	}

	printf("check\n");

	for(int i=0; i<30;i++){
	
	//send(clientSocket , hellocheck ,sizeof(hellocheck), 0 );
	printf("Files: info1\tinfo2\tinfo3\nEnter File name from which data is to be fetched:\n");
	scanf("%s",filename);

	//send(clientSocket , hellocheck ,sizeof(hellocheck), 0 );
	send(clientSocket , filename ,sizeof(filename), 0 );
	
	valread = read( clientSocket , buffer, MAXSIZE);
	buffer[valread]='\0';
	printf("Msg Received:%s\n",buffer );
	}	
	close(clientSocket);
return 0;
}