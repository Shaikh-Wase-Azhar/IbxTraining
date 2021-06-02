#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>


#define UDPPORT 9090
#define MAXLINE 1024

int main(int argc, char const *argv[])
{
	int sockfd;
	char buffer[MAXLINE] = {0};
	char *hello = "C:Hello from  C udp server:..";

	struct sockaddr_in servadd,cliaddr;
	
	// Creating socket file descriptor
	if ((sockfd = socket(AF_INET, SOCK_DGRAM , 0)) == 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}
	
	memset(&servadd,0,sizeof(servadd));
	memset(&cliaddr,0,sizeof(cliaddr));


	//Filing server information
	servadd.sin_family = AF_INET;  //ipv4
	servadd.sin_addr.s_addr = INADDR_ANY;
	servadd.sin_port = htons( UDPPORT );
	
	//Bind the socket with server address
	if (bind(sockfd, (struct sockaddr *)&servadd,
								sizeof(servadd))<0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	int len ,n;

	len= sizeof(cliaddr); //len is value/result
	// MSG_WAITALL wait untill data is recieved..
	n= recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr*)& cliaddr,&len);
	buffer[n]='\0';
	printf("C: %s\n",buffer);
	//sendto(sockfd,(const char*)hello,strlen(hello),MSG_CONFIRM, (struct sockaddr*)&cliaddr, len);
	printf("C:Message recieved\n");
	return 0;
}
