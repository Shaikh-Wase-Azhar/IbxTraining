#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>

#define PORT 8080
#define INPUTFILE1 "/home/wase/myall/innobox/5day/info1.txt"
#define INPUTFILE2 "/home/wase/myall/innobox/5day/info2.txt"
#define INPUTFILE3 "/home/wase/myall/innobox/5day/info3.txt"
FILE *fp1=NULL;
FILE *fp2=NULL;
FILE *fp3=NULL;

int create_srvrsock();
void* socketThread(void*);
int Req_Handling(char*,int);
int File_oper(FILE*,int);

int create_srvrsock(){
    int serverSocket=-1,clientSocket=-1;
    struct sockaddr_in serverAddr,clientAddr;
	socklen_t len;
	pthread_t tid[10];
	int index=0;

    if((serverSocket = socket(AF_INET, SOCK_STREAM, 0))<0){
		printf("\n Socket creation error \n");
		return -1;
	}

    serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	//serverAddr.sin_addr.s_addr = htonl("127.0.0.1"));
	serverAddr.sin_port = htons(PORT);

    bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

    if(listen(serverSocket,3)==0)
	printf("Listening....\n");
	else
	printf("Error, Limit over\n");

    len=sizeof(clientAddr);

    while(1){
        if((clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr,&len))<0){
            perror("accept");
            exit(EXIT_FAILURE);
        }
        else{
            if(pthread_create(&tid[index++], NULL, socketThread,&clientSocket) != 0 )
            printf("Failed to create thread\n");
        }
    }	
}

void* socketThread(void* clntSocket){
    int clientSocket=*(int*)clntSocket;
    while(1){
        char filename[10]={0};
        recv(clientSocket,filename,10,0);
        printf("filename recieved is: %s\n", filename);
        if(Req_Handling(filename,clientSocket)==-1){
            close(clientSocket);
            break;
        }
	}
}

int Req_Handling(char *filename,int clientSocket){
    printf("filename recieved is: %s\n", filename);
    if(!strcmp(filename, "info1")){
        fp1=fopen(INPUTFILE1,"r");
		File_oper(fp1,clientSocket);
    }
    else if(!strcmp(filename, "info2")){
        fp2=fopen(INPUTFILE2,"r");
		File_oper(fp2,clientSocket);
    }
	else if(!strcmp(filename, "info3")){
        fp3=fopen(INPUTFILE3,"r");
		File_oper(fp3,clientSocket);
    }
	else{
        send(clientSocket,"Wrong Filename...\n",20,0);
        return 0;
    }
return 0;
}

int File_oper(FILE *fptr,int clientSocket){
    char line[20]={0};
	if(fptr!=NULL){
       	printf("file opened successfully...\n");
       	while(fgets(line,sizeof(line),fptr)!=NULL){
            line[strlen(line)-1]='\0';
       		send(clientSocket,line,sizeof(line),0);
       	}  
        printf("1 line/message sent to client %d...\n",clientSocket);   	
    }
    else if(fptr==NULL){
		perror("open file failed\n");
		return 0;
	}
    return 0;
}

int main(){
    create_srvrsock();
    return 0;
}