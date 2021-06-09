 // Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>  

#define PORT 8080
#define MAXLINE 1024

int main(int argc, char const *argv[])
{
    int sockfd;
    char buffer[MAXLINE] = {0};
	char *hello = "Hello from udp client:";
	struct sockaddr_in serv_addr;
	
    //creating sockey file descriptor
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		printf("\n Socket creation error \n");
		return -1;
	}

    memset(&servadd,0,sizeof(servadd));

    servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;

	int n len;

    len= sizeof(servaddr); //len is value/result

    sendto(sockfd,(const char*)hello,strlen(hello),MSG_CONFIRM, (const struct sockaddr*)&servaddr, sizeof(servaddr));
    printf("Hello msg sent\n");
    // MSG_WAITALL wait untill data is recieved..
    n= recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr*)& servaddr,&len);
    buffer[n]='\0';
    printf("Server: %s\n",buffer);
    close(sockfd);
    
    return 0;
}

