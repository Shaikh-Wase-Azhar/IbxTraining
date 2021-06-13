#include <stdio.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define PORT 8080
#define MAXSIZE 1024

//FILE *fp1= fopen("/home/wase/myall/innobox/5day/info1.txt","a+");
//FILE *fp2=fopen("/home/wase/myall/innobox/5day/info2.txt","a+");
//FILE *fp3=fopen("/home/wase/myall/innobox/5day/info3.txt","a+");

int main(int argc, char const *argv[])
{
  //  char dataToBeWritten[50] = "hello its trial";   
	//fputs(dataToBeWritten, fp) ;     
    //fputs("\n", fp) ;   
	int clientSocket = 0, valread;
	struct sockaddr_in serverAddr;
	char buffer[MAXSIZE] = {0};
	
	if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("\n Socket creation error \n");
		return -1;
	}

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);

	//memory set to zero
	memset(&serverAddr, 0, sizeof(serverAddr));

	// Convert IPv4 and IPv6 addresses from text to binary form
	if(inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr)<=0)
	{
		printf("\nInvalid address/ Address not supported \n");
		return -1;
	}

	if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
	{
		printf("\nConnection Failed \n");
		return -1;
	}

	printf("check\n");
	
	//send(clientSocket , fp , 16, 0 );

	char *hellocheck = "Hellocheck";
	send(clientSocket , hellocheck ,sizeof(hellocheck), 0 );
	printf("Message sent to server\n");
	
	printf("check\n");

	//valread = read( sock , buffer, 1024);
	//printf("%s\n",buffer );
	//while(1) {
	//	sleep(1);
	//	valread = read( sock , buffer, 1024);
	//	if(valread<=0)  
	  //  return 0;	
	//	printf("%s\n",buffer );
	//}
   
	//fclose(fp1);
	//fclose(fp2);
	//fclose(fp3);
	close(clientSocket);
	return 0;
}