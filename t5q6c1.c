#include <stdio.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define PORT 8080

int main(int argc, char const *argv[]){
	int clientSocket = 0, valread;
	struct sockaddr_in serverAddr;
	char line[20] = {0};
	char filename[10]= {0};

	//memory set to zero
	memset(&serverAddr, 0, sizeof(serverAddr));

	if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		printf("\n Socket creation error \n");
		return -1;
	}

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	// Convert IPv4 and IPv6 addresses from text to binary form
	if(inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr)<=0){
		printf("\nInvalid address/ Address not supported \n");
		return -1;
	}

	if(connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0){
		printf("\nConnection Failed \n");
		return -1;
	}

	for(int i=0; i<30;i++){
		printf("Files: info1\tinfo2\tinfo3\nEnter File name from which data is to be fetched:");
		scanf("%s",filename);

		//Sending filename to Server
		send(clientSocket , filename ,sizeof(filename), 0 );
	

		valread = recv(clientSocket,line,sizeof(line),0);
		line[valread]='\0';
		printf("Msg Received:%s\n\n",line);
	}	
	close(clientSocket);
return 0;
}