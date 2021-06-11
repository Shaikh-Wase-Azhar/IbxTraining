#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#define PORT 8080

int main(int argc, char const *argv[])
{
  //  char dataToBeWritten[50] = "hello its trial"; //

    FILE *fp=fopen("/home/wase/myall/innobox/5day/info1.txt","a+");

    //fputs(dataToBeWritten, fp) ;  //   
    //fputs("\n", fp) ;   //

	int sock = 0, valread;
	struct sockaddr_in serv_addr;
	char *hello = "Hello from client1...";
	char buffer[1024] = {0};
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("\n Socket creation error \n");
		return -1;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	
	// Convert IPv4 and IPv6 addresses from text to binary form
	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
	{
		printf("\nInvalid address/ Address not supported \n");
		return -1;
	}

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("\nConnection Failed \n");
		return -1;
	}


	//int linelen;
	//for(linelen=0;fp[linelen]!='\n';linelen++);
	//printf("line lenght is:%d\n",linelen);
	send(sock , fp , 16, 0 );
	printf("Message sent to server\n");
	//valread = read( sock , buffer, 1024);
	//printf("%s\n",buffer );
	//while(1) {
	//	sleep(1);
	//	valread = read( sock , buffer, 1024);
	//	if(valread<=0)  
	  //  return 0;	
	//	printf("%s\n",buffer );
	//}
   
   // fclose(fp);
	return 0;
}