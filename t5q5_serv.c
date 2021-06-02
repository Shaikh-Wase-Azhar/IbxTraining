// Server side C/C++ program to demonstrate Socket programming
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arps/inet.h>


#define PORT 8080
#define MAXLINE 1024
#define PORT2 9090

int main(int argc, char const *argv[])
{
	int sockfd;
    int sockfd2;
	char buffer[MAXLINE];
    char buffer2[MAXLINE];
	char *hello = "Hello from udp server:..";

	struct sockaddr_in servadd,cliaddr;
    struct sockaddr_in servadd2,cliaddr2;
	
	// Creating socket file descriptor
	if ((sockfd = socket(AF_INET, SOCK_DGRAM , 0)) == 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}
	
    // Creating socket file descriptor
	if ((sockfd2  = socket(AF_INET, SOCK_DGRAM , 0)) == 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	memset(&servadd2,0,sizeof(servadd2));
	memset(&cliaddr2,0,sizeof(cliaddr2));

    memset(&servadd,0,sizeof(servadd));
	memset(&cliaddr,0,sizeof(cliaddr));

	//Filing server information
	servaddr.sin_family = AF_INET;  //ipv4
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons( PORT );
	
    //Filing server2 information
	servaddr2.sin_family = AF_INET;  //ipv4
	servaddr2.sin_addr.s_addr = INADDR_ANY;
	servaddr2.sin_port = htons( PORT2 );


	//Bind the socket with server address
	if (bind(serverfd, (cont struct sockaddr *)&servaddr,
								sizeof(servaddr))<0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

    //Bind the socket with server address
	if (bind(serverfd, (cont struct sockaddr *)&servaddr2,
								sizeof(servaddr2))<0)
	{
		perror("bind failed for 2nd sever");
		exit(EXIT_FAILURE);
	}

int len ,n;
 
len= sizeof(cliaddr); //len is value/result
len2= sizeof(cliaddr2);
// MSG_WAITALL wait untill data is recieved..
n= recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr*)&cliaddr,&len);
buffer[n]='\0';
printf("Client : from client1 of server1 %s\n",buffer);

n= recvfrom(sockfd2, (char *)buffer2, MAXLINE, MSG_WAITALL, (struct sockaddr*)&cliaddr2,&len2);
buffer2[n]='\0';
printf("Client2:  from client2 of server1 %s\n",buffer2);

sendto(sockfd,(const char*)hello,strlen(hello),MSG_CONFIRM, (const struct sockaddr*)&cliaddr, len);
sendto(sockfd2,(const char*)hello,strlen(hello),MSG_CONFIRM, (const struct sockaddr*)&cliaddr2, len2);
printf("Hello msg sent\n");
	return 0;
}

