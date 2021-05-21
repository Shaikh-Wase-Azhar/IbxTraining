#include<stdio.h>
#include<unistd.h>

#define READ 0
#define WRITE 1

int main() {
   int pipefds1[2], pipefds2[2];
   char p1wr[20];
   char p2wr[20];
   char buff[20];
      
   if (pipe(pipefds1) == -1) {
      printf("Unable to create pipe 1 \n");
      return 1;
   }
   if (pipe(pipefds2) == -1) {
      printf("Unable to create pipe 2 \n");
      return 1;
   }

    pid_t id= fork();
   if (id != 0) {// Parent process 
	      close(pipefds1[READ]); // Close the unwanted pipe1 read side
	      close(pipefds2[WRITE]); // Close the unwanted pipe2 write side
          printf("Parent:Enter the msg:");
          scanf("%s",p1wr);  
	      printf("Parent: Writing to pipe 1 – Message is %s\n", p1wr);
	      write(pipefds1[WRITE], p1wr, sizeof(p1wr));
	      read(pipefds2[READ], buff, sizeof(buff));
	      printf("Parent: Reading from pipe 2 – Message is %s\n", buff);
   } else { //child process
	      close(pipefds1[WRITE]); // Close the unwanted pipe1 write side
	      close(pipefds2[READ]); // Close the unwanted pipe2 read side
	      read(pipefds1[READ], buff, sizeof(buff));
	      printf("Child: Reading from pipe 1 – Message is %s\n",buff);
          printf("Child:Enter the msg:");
          scanf("%s",p2wr);  
	      printf("In Child: Writing to pipe 2 – Message is %s\n", p2wr);
	      write(pipefds2[WRITE], p2wr, sizeof(p2wr));
   }
   return 0;
}
