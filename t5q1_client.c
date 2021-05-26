// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#include <pthread.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>

#define PORT 8080
int sock = 0, valread;
struct sockaddr_in serv_addr;
char *hello = "Hello from client:Naveen";
char buffer[1024] = {0};

pthread_barrier_t barrier;

void * routine(void * arg)
{
printf("Waiting at the barrierf......\n");
pthread_barrier_wait(&barrier);

//sending connect request all at same time to server
if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{printf("\n Socket creation error \n");
		return -1;}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	
	// Convert IPv4 and IPv6 addresses from text to binary form
	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
	{printf("\nInvalid address/ Address not supported \n");
		return -1;}

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{printf("\nConnection Failed \n");
		return -1;}

	send(sock , hello , strlen(hello) , 0 );
	printf("Hello message sent\n");
	valread = read( sock , buffer, 1024);
	printf("%s\n",buffer );
	while(1) {
		sleep(1);
		valread = read( sock , buffer, 1024);
		if(valread<=0)
			return 0;	
		printf("%s\n",buffer );
	}
} 

int main(int argc, char const *argv[])
{
     pthread_t th[5];
    pthread_barrier_init(&barrier,NULL,5);
    int i;
    for (i=0; i<5; i++)
        if(pthread_create(&th[i],NULL,&routine , NULL)!=0)
        perror("Failed to create thread");
    
    for (i=0; i<5; i++)
        if(pthread_join(th[i],NULL)!=0)
        perror("Failed to join thread");
    pthread_barrier_destroy(&barrier);

	return 0;
}

