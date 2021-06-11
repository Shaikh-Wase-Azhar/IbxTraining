#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <signal.h>
#include <arpa/inet.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close
#include <pthread.h>
#define PORT 8080
#define MAXLINE 1024

/*int max(int x, int y)
{
	if (x > y)
		return x;
	else
		return y;
}
*/

char client_message[2000];
char buffer[1024];
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void * socketThread(void *arg)
{
  int newSocket = *((int *)arg);
  recv(newSocket , client_message , 2000 , 0);

  
   while(1) {
	printf("Waiting on socket %d\n",newSocket);
	sleep(2);

   }			
}

int main(){
  int serverSocket, newSocket,maxfdp1;;
  struct sockaddr_in serverAddr,cliaddr;
  struct sockaddr_storage serverStorage;
  socklen_t addr_size;

	char buffer[MAXLINE];
	pid_t childpid;
	fd_set rset;
	socklen_t len;
	char* message = "Hello Client";

//  FILE *fp=fopen("/home/wase/myall/innobox/5day/info1.txt","r");

// create listening TCP socket 
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	bzero(&serverAddr, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddr.sin_port = htons(PORT);

// binding server addr structure to listenfd
	bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	listen(serverSocket, 10);

  //Set all bits of the padding field to 0 
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

  //Bind the address struct to the socket 
  bind(serverSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

// clear the descriptor set
	FD_ZERO(&rset);
  //maxfdp1 = max(serverSocket) + 1;
 maxfdp1 = serverSocket + 1;
for (;;) {
		// set listenfd and udpfd in readset
		FD_SET(serverSocket, &rset);

		// select the ready descriptor
    select(maxfdp1, &rset, NULL, NULL, NULL);

		// if tcp socket is readable then handle
		// it by accepting the connection
		if (FD_ISSET(serverSocket, &rset)) {
			//len = sizeof(cliaddr);
      len = sizeof(serverAddr);
			//newSocket = accept(serverSocket, (struct sockaddr*)&cliaddr, &len);
      newSocket = accept(serverSocket, (struct sockaddr*)&serverAddr, &len);
      
      if ((childpid = fork()) == 0) {
				close(serverSocket);
			//	bzero(buffer, sizeof(buffer));
				printf("Message From TCP client %d is: ", newSocket);
				memset(buffer, 0, sizeof(buffer));
        read(newSocket, buffer, sizeof(buffer));
				puts(buffer);
				//write(newSocket, (const char*)message, sizeof(buffer));
				close(newSocket);
				exit(0);
			}
			close(newSocket);
		}
	}
////

/*
  //Listen on the socket, with 40 max connection requests queued 
  if(listen(serverSocket,50)==0)
    printf("Listening\n");
  else
    printf("Error\n");
    pthread_t tid[60];
    int i = 0;
    while(1)
    {
        //Accept call creates a new socket for the incoming connection
        addr_size = sizeof serverStorage;
        newSocket = accept(serverSocket, (struct sockaddr *) &serverStorage, &addr_size);

        read(newSocket, buffer, sizeof(buffer));
				printf("Data received from client %d is:",newSocket);
        puts(buffer);

        //for each client request creates a thread and assign the client request to it to process
       //so the main thread can entertain next request
        if( pthread_create(&tid[i++], NULL, socketThread, &newSocket) != 0 )
           printf("Failed to create thread\n");

        if( i >= 50)
        {
          i = 0;
          while(i < 50)
          {
            pthread_join(tid[i++],NULL);
          }
          i = 0;
        }
    }*/
  return 0;
}