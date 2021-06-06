#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>


sem_t s1,s2;

int main() {
   int fd;
   char readbuf[80];
   char end[10];
   int to_end;
   int read_bytes;
   
   char p1wr[50];
   char p2wr[50];

   sem_init(&s1,0,1);
   sem_init(&s2,0,0);

  
    mkfifo("fifo", S_IRUSR | S_IWUSR);
    fd = open("fifo", O_RDWR);
   
 
     pid_t id= fork();
   while(1){
      if (id != 0) {// Parent process 
         printf("Parent:Enter the msg:");
         scanf("%s",p1wr);  
	      write(fd, p1wr, strlen(p1wr));
         printf("Parent:Sent string: \"%s\" and string length is %d\n", p1wr, (int)strlen(p1wr));
        

         read_bytes = read(fd, p2wr, sizeof(p2wr));
         p2wr[read_bytes] = '\0';
         printf("Parent:Received string: \"%s\" and length is %d\n\n", p2wr, (int)strlen(p2wr));
       //  to_end = strcmp(readbuf, end);
        // if (to_end == 0) {
        // close(fd);}
              }

   else { //child process
	      
         read_bytes = read(fd, p1wr, sizeof(p1wr));
         p1wr[read_bytes] = '\0';
         printf("Child:Received string: \"%s\" and length is %d\n\n", p1wr, (int)strlen(p1wr));
      //   to_end = strcmp(readbuf, end);
        // if (to_end == 0) {
        // close(fd);}
      

         printf("Child:Enter the msg:");
         scanf("%s",p2wr);  
	      write(fd, p2wr, strlen(p2wr));
         printf("Child:Sent string: \"%s\" and string length is %d\n", p2wr, (int)strlen(p2wr));      
         
        }
   }
   
return 0;
}