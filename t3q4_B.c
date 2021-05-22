#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

/*
#define FIFO_FILE1 "MYFIFO1"
#define FIFO_FILE2 "MYFIFO2"
int main() {
   char readbuf[80];
   char end[10];
   int to_end;
   int read_bytes;
   
   // Create the FIFO if it does not exist 
   mknod(FIFO_FILE, S_IFIFO|0640, 0);
   strcpy(end, "end");

   //printf("CLIENT_B: Send messages to C, infinitely, to end enter \"end\"\n");
   //int fd = open(FIFO_FILE2, O_CREAT|O_WRONLY);
   //strcpy(end_str, "end");

   while(1) {
      int fd1 = open(FIFO_FILE1, O_RDONLY);
      memset(readbuf,0,sizeof(readbuf)); 	
      read_bytes = read(fd, readbuf, sizeof(readbuf));
      readbuf[read_bytes] = '\0';
      printf("CLIENT_B:Received string from A: \"%s\" and length is %d\n", readbuf, (int)strlen(readbuf));
      to_end = strcmp(readbuf, end);
      if (to_end == 0) {
         close(fd);
         break;
      }
   }

   printf("CLIENT_B: Send messages to C, infinitely, to end enter \"end\"\n");
   int fd2 = open(FIFO_FILE2, O_CREAT|O_WRONLY);
   strcpy(end_str, "end");

   return 0;
}
*/



int main()
{
   unsigned char buff[50];
   mkfifo("fifo1", S_IRUSR | S_IWUSR);
   int fd= open("fifo1", O_RDWR);
   read(fd, buff ,sizeof(buff));
   printf("CLIENT B: Recvd data is :%s\n",buff);
   close(fd);

   mkfifo("fifo2", S_IRUSR | S_IWUSR);
   fd= open("fifo2", O_WRONLY);
   write(fd,buff,sizeof(buff));
   close(fd);
   return 0;
}






