#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define FIFO_FILE "MYFIFO"
int main() {
   int fd;
   char readbuf[80];
   char end[10];
   int to_end;
   int read_bytes;
   
   char p1wr[20];
   char p2wr[20];
   
   /* Create the FIFO if it does not exist */
   mknod(FIFO_FILE, S_IFIFO|0640, 0);
   strcpy(end, "end");

   fd = open(FIFO_FILE, O_RDONLY);
      memset(readbuf,0,sizeof(readbuf)); 	

    pid_t id= fork();
   if (id != 0) {// Parent process 
	      
         printf("Parent:Enter the msg:");
         scanf("%s",p1wr);  
	      write(fd, p1wr, strlen(p1wr));
         printf("Parent:Sent string: \"%s\" and string length is %d\n", p1wr, (int)strlen(p1wr));

         /*read_bytes = read(fd, readbuf, sizeof(readbuf));
         readbuf[read_bytes] = '\0';
         printf("Received string: \"%s\" and length is %d\n", readbuf, (int)strlen(readbuf));
         to_end = strcmp(readbuf, end);
         if (to_end == 0) {
         close(fd);
      }*/

   } else { //child process
	      read_bytes = read(fd, readbuf, sizeof(readbuf));
         readbuf[read_bytes] = '\0';
         printf("Received string: \"%s\" and length is %d\n", readbuf, (int)strlen(readbuf));
         to_end = strcmp(readbuf, end);
         if (to_end == 0) {
         close(fd);
      }

         /*printf("Parent:Enter the msg:");
         scanf("%s",p2wr);  
	      write(fd, p2wr, strlen(p2wr));
         printf("Parent:Sent string: \"%s\" and string length is %d\n", p2wr, (int)strlen(p2wr));
      */
     }
   return 0;
}


  
   
   









