#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <fcntl.h> 
#include <unistd.h> 
#include <pthread.h>

#define PORT 8080
#define MAXSIZE 1024
//#define INADDR_ANY  "127.0.0.1"

int serverSocket,newSocket,maxfdp1;
struct sockaddr_in serverAddr,cliaddr;
char buffer[MAXSIZE]= {0};
char filename[10]= {0};
fd_set rset;
socklen_t len;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

FILE *fp1,*fp2,*fp3;

void * socketThread()
{  

	printf("check2\n");
	pthread_mutex_lock(&lock);
		///// set ServerSocket in readset
		FD_SET(serverSocket, &rset);

	for(;;){
		//////// set ServerSocket in readset
		FD_SET(serverSocket, &rset);

		//////// select the ready descriptor
    	select(maxfdp1, &rset, NULL, NULL, NULL);

    	printf("Server:Send the Filename.....");
			
		// if tcp socket is readable then handle
		// it by accepting the connection
		if (FD_ISSET(serverSocket, &rset)) {
			len = sizeof(cliaddr);
      
			if ((newSocket = accept(serverSocket, (struct sockaddr *)&cliaddr,
						(socklen_t*)&cliaddr))<0)
		{
			perror("accept");
			exit(EXIT_FAILURE);
		}
			memset(buffer, 0, MAXSIZE );
			memset(filename, 0, 10 );

      		//recv(newSocket , buffer , 2000 , 0);  
      		read(newSocket, filename, 10);
			
			if(!strcmp(filename, "info1")) fgets(buffer, MAXSIZE,fp1);
			else if(!strcmp(filename, "info2")) fgets(buffer, MAXSIZE,fp2);
			else if(!strcmp(filename, "info3")) fgets(buffer, MAXSIZE,fp3);
			else strcpy(buffer,"Wrong Filename...");

        	write(newSocket, (const char*)buffer, MAXSIZE);
			
			pthread_mutex_unlock(&lock);
			close(newSocket);
		}
	}			
}


int main(){

	fp1=fopen("/home/wase/myall/innobox/5day/info1.txt","a+");
	fp2=fopen("/home/wase/myall/innobox/5day/info2.txt","a+");
	fp3=fopen("/home/wase/myall/innobox/5day/info3.txt","a+");

	// create listening TCP socket 
	if((serverSocket = socket(AF_INET, SOCK_STREAM, 0))<0)
  {
		printf("\n Socket creation error \n");
		return -1;
	}

	//bzero(&serverAddr, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddr.sin_port = htons(PORT);

	//memory set to zero
	memset(&serverAddr, 0, sizeof(serverAddr));
	memset(&cliaddr, 0, sizeof(cliaddr));

	// binding server addr structure to serverSocket
	bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	listen(serverSocket, 10);

  	//Set all bits of the padding field to 0 
 	memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

	///////// clear the descriptor set
	FD_ZERO(&rset);

	/////////Max fd to select
 	maxfdp1 = serverSocket + 1;

	
	//Listen on the socket, with 3 max connection requests queued 
	if(listen(serverSocket,3)==0)
	printf("Listening....\n");
	else
	printf("Error, Limit over\n");
	

	pthread_t tid[3];

int i=0;
for(;;)
    {
      printf("check1\n");

        //Accept call creates a new socket for the incoming connection
        //addr_size = sizeof cliaddr;
        /*if ((newSocket = accept(serverSocket, (struct sockaddr *)&cliaddr,
						(socklen_t*)&cliaddr))<0)
		{
			perror("accept");
			exit(EXIT_FAILURE);
		}*/
		
        //creating thread for each client
        if( pthread_create(&tid[i++], NULL, socketThread, NULL) != 0 )
           printf("Failed to create thread\n");

        if( i >= 3)
        {
          i = 0;
          while(i < 3)
          {
            pthread_join(tid[i++],NULL);
          }
          i = 0;
        }
    }

	fclose(fp1);
	fclose(fp2);
	fclose(fp3);

  return 0;
}