#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080

int main(){

char client_message[2000];
char buffer[1024];
int serverSocket,newSocket;
    struct sockaddr_in serverAddr;
    struct sockaddr_storage serverStorage;
    socklen_t addr_size;

    //creating socket

    serverSocket=socket(AF_INET,SOCK_STREAM,0);

    //configure settings of server sockaddr struct

    serverAddr.sin_family=AF_INET;
    serverAddr.sin_port=htons(PORT);

    if(inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr)<=0)
	{
		printf("\nInvalid address/ Address not supported \n");
		return -1;
	}
    
    //setting all bits of padding field to 0

    memset(serverAddr.sin_zero,'\0',sizeof(serverAddr.sin_zero));

    //Binding 

    bind(serverSocket,(struct sockaddr *)&serverAddr,sizeof(serverAddr));

    //Listen

    if(listen(serverSocket,3)==0)
    {
        printf("Listening......\n");
    }
    else
    {
        printf("Error.......\n");
    }

    //Accept

    addr_size=sizeof(serverStorage);


	while(1)
    {
		newSocket = accept(serverSocket, (struct sockaddr*)&serverStorage, &addr_size);
        fork();
		close(serverSocket);
		while(1)
        {
			recv(newSocket, buffer, 1024, 0);
			printf("Client Message: %s\n", buffer);
            printf("Enter the message:\n");
            fgets(client_message,1000,stdin);
			send(newSocket,client_message, 1000, 0);
		}
	}

	close(newSocket);
	return 0;
}
