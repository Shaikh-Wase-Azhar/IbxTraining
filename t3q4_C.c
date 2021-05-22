#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>


#define FIFO_FILE2 "MYFIFO2"
int main() {
   char readbuf[80];
   char end[10];
   int to_end;
   int read_bytes;
   
   // Create the FIFO if it does not exist 
   mknod(FIFO_FILE2, S_IFIFO|0640, 0);
   strcpy(end, "end");
   
      int fd2 = open(FIFO_FILE2, O_RDONLY);
      memset(readbuf,0,sizeof(readbuf)); 	
      read_bytes = read(fd2, readbuf, sizeof(readbuf));
      readbuf[read_bytes] = '\0';
      printf("CLIENT_C:Received string from C: \"%s\" and length is %d\n", readbuf, (int)strlen(readbuf));
      to_end = strcmp(readbuf, end);
      if (to_end == 0) {
         close(fd2);
      }
   return 0;
}


/*
int main()
{
   unsigned char buff[50];
   mkfifo("fifo2", S_IRUSR | S_IWUSR);
   int fd= open("fifo2", O_RDONLY);
   read(fd, buff ,sizeof(buff));
   printf("CLIENT C: Recvd data is :%s\n",buff);
   close(fd);
   return 0;
}
*/
