#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close
#include <pthread.h>
#define PORT 8080
#define MAXSIZE 1024


int serverSocket, newSocket,maxfdp1;
struct sockaddr_in serverAddr,cliaddr;
char buffer[MAXSIZE]= {0};
fd_set rset;
socklen_t len;

/*
FILE *fp1=fopen("/home/wase/myall/innobox/5day/info1.txt","r");
FILE *fp2=fopen("/home/wase/myall/innobox/5day/info2.txt","r");
FILE *fp3=fopen("/home/wase/myall/innobox/5day/info3.txt","r");
*/

void * socketThread()
{  
for (;;)
 {
		// set ServerSocket in readset
		FD_SET(serverSocket, &rset);

		// select the ready descriptor
    select(maxfdp1, &rset, NULL, NULL, NULL);

    printf("Server:Send the Message.....");
			
		// if tcp socket is readable then handle
		// it by accepting the connection
		if (FD_ISSET(serverSocket, &rset)) {
			len = sizeof(cliaddr);
      
			newSocket = accept(serverSocket, (struct sockaddr*)&cliaddr, &len);
      
			//	bzero(buffer, MAXSIZE);
			printf("Message From TCP client %d is: ", newSocket);
			memset(buffer, 0, MAXSIZE );

      //recv(newSocket , buffer , 2000 , 0);  
      read(newSocket, buffer, MAXSIZE);
			puts(buffer);
				
      //write(newSocket, (const char*)message, MAXSIZE);
			close(newSocket);
		}
	}			
}


int main(){

  int serverSocket=0, newSocket,maxfdp1;

  struct sockaddr_storage serverStorage;
  socklen_t addr_size;

//FILE *fp=fopen("/home/wase/myall/innobox/5day/info1.txt","r");

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
  //memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

// clear the descriptor set
	FD_ZERO(&rset);

//Max fd to select
 maxfdp1 = serverSocket + 1;

//Listen on the socket, with 40 max connection requests queued 
if(listen(serverSocket,3)==0)
printf("Listening....\n");
else
printf("Error, Limit over\n");

pthread_t tid[3];

int i;
for(i=0;i<3;i++)
    {
      printf("check1\n");
        //Accept call creates a new socket for the incoming connection
        //addr_size = sizeof cliaddr;
        if ((newSocket = accept(serverSocket, (struct sockaddr *)&cliaddr,
						(socklen_t*)&cliaddr))<0)
		{
			perror("accept");
			exit(EXIT_FAILURE);
		}
        //read(newSocket, buffer, sizeof(buffer));
				//printf("Data received from client %d is:",newSocket);
        //puts(buffer);

printf("check2\n");
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
/*
  fclose(fp1);
	fclose(fp2);
	fclose(fp3);
*/
  return 0;
}