//Udp Server+Threading+Files
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <sys/stat.h>
#include <fcntl.h>

#define PORT 8010

int socketfd,selectfd,maxfd;
fd_set rset;
char sender[1000];
struct sockaddr_in serverAddr, clientAddr;
int len;
FILE *fileFd[3];

//Structure to receive data
struct main
{
    char file[1000];
    int command;
};
struct main data;

void *fun()
{
    fileFd[0]=fopen("Gnany.txt","r");
    fileFd[1]=fopen("Abhi.txt","r");
    fileFd[2]=fopen("Nani.txt","r");
    //Setting Read Set for Server
    FD_SET(socketfd,&rset);
    len = sizeof(clientAddr);
    while(1)
    {
        //Select
        selectfd=select(maxfd, &rset, NULL, NULL,NULL);
        if(selectfd==0)
        {
            printf("Timed out\n");
        }
        else if(selectfd==-1)
        {
            printf("Eror in Select");
        }
        else
        {
            if((FD_ISSET(socketfd, &rset)))
            {
                //Reading
                if(recvfrom(socketfd, (struct main *)&data, sizeof(data),0,(struct sockaddr*)&clientAddr,&len)==0)
                {
                    printf("Client Closed or No data to read\n");
                }
                else
                {
                    if(strcmp(data.file,"Gnany")==0)
                    {
                        fgets(sender,sizeof(sender),fileFd[0]);
                    }
                    else if(strcmp(data.file,"Abhi")==0)
                    {
                        fgets(sender,sizeof(sender),fileFd[1]);
                    }
                    else if(strcmp(data.file,"Nani")==0)
                    {
                        fgets(sender,sizeof(sender),fileFd[2]);
                    }
                    else
                    {
                        strcpy(sender,"Give proper Input");
                    }
                    //Writing
                    sendto(socketfd,sender,sizeof(sender),0,(struct sockaddr*)&clientAddr,len);
                    printf("Send Data\n");
                }
            }
        }     
    }
}

int main() {
	// Creating socket file descriptor
	if ( (socketfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}

	
	memset(&serverAddr, 0, sizeof(serverAddr));
	memset(&clientAddr, 0, sizeof(clientAddr));
	
	// Filling server information
	serverAddr.sin_family = AF_INET; // IPv4
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_port = htons(PORT);

    //Clearing the descriptor set
    FD_ZERO(&rset);

    //Get max for Select
    maxfd=(socketfd+1);
	
	// Bind the socket with the server address
	if ( bind(socketfd,(const struct sockaddr *)&serverAddr,sizeof(serverAddr)) < 0 )
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
    
    pthread_t tid[3];

    //Creating Thread
    for(int i=0;i<3;i++)
    {
        if(pthread_create(&tid[i++],NULL,fun,NULL) !=0)
        {
            printf("Failed to create Thread\n");
        }
       if( i >=3)
        {
          i = 0;
          while(i <= 3)
          {
            pthread_join(tid[i++],NULL);
          }
          i = 0;
        }
    }
    fclose(fileFd[0]);
    fclose(fileFd[1]);
    fclose(fileFd[2]);
    close(socketfd);
}
