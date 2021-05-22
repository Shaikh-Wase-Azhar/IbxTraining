#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>


#define FIFO_FILE1 "MYFIFO1"
#define FIFO_FILE2 "MYFIFO2"
int main() {
   char readbuf[80];
   char end[10];
   int to_end;
   int read_bytes;

   char  end_str[5];
   
   // Create the FIFO if it does not exist 
   mknod(FIFO_FILE1, S_IFIFO|0640, 0);
   strcpy(end, "end");

   int fd2 = open(FIFO_FILE2, O_CREAT|O_WRONLY);
   strcpy(end_str, "end");

      int fd1 = open(FIFO_FILE1, O_RDONLY);
      memset(readbuf,0,sizeof(readbuf)); 	
      read_bytes = read(fd1, readbuf, sizeof(readbuf));
      readbuf[read_bytes] = '\0';
      printf("CLIENT_B:Received string from A: \"%s\" and length is %d\n", readbuf, (int)strlen(readbuf));
      to_end = strcmp(readbuf, end);
      
      if (to_end == 0) {
         close(fd1);
      }
      int stringlen = strlen(readbuf);
      readbuf[stringlen - 1] = '\0';
      int end_process = strcmp(readbuf, end_str);
      
      //printf("end_process is %d\n", end_process);
      if (end_process != 0) {
         write(fd2, readbuf, strlen(readbuf));
         printf("CLIENT_B:Sent string to C: \"%s\" and string length is %d\n", readbuf, (int)strlen(readbuf));
      } else {
         write(fd2, readbuf, strlen(readbuf));
         printf("CLIENT_B:Sent string to C: \"%s\" and string length is %d\n", readbuf, (int)strlen(readbuf));
         close(fd1);
         close(fd2);
      }
   return 0;
}



/*
int main()
{
   unsigned char buff[50];
   mkfifo("fifo1", S_IRUSR | S_IWUSR);
   int fd1= open("fifo1", O_RDWR);
   read(fd1, buff ,sizeof(buff));
   printf("CLIENT B: Recvd data is :%s\n",buff);
   close(fd1);
   mkfifo("fifo2", S_IRUSR | S_IWUSR);
   int fd2= open("fifo2", O_WRONLY);
   write(fd2,buff,sizeof(buff));
   close(fd2);
   return 0;
}
*/
