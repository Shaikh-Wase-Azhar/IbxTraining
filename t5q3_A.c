#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#define PORT 8080

int main()
{
    int sock = 0, valread;
	struct sockaddr_in serv_addr;
	char buffer[1024] = {0};
	char input[100];
    printf("A:Enter a string:\n");
    fgets(input,100,stdin);
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		printf("\n Socket creation error \n");	
		return -1;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
	{
		printf("\nInvalid address/ Address not supported \n");
		return -1;
	}

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) != 0)
	{
		printf("\nConnection Failed \n");
		return -1;
	}
	send(sock , input , strlen(input) , 0 );
	printf("A:Message sent to B\n");
    return 0;
}