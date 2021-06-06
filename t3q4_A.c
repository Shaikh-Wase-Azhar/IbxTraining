#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

////#define FIFO_FILE1 "MYFIFO1"


int main() {
   int end_process;
   int stringlen;
   char readbuf[80];
   char end[10];
   char end_str[5];
   printf("CLIENT_A: Send messages, infinitely, to end enter \"end\"\n");
   
  // mknod(FIFO_FILE1, S_IFIFO|0640, 0);
   //strcpy(end, "end");

   ////int fd1 = open(FIFO_FILE1, O_CREAT|O_WRONLY|S_IRWXU);
   mkfifo("FIFOAB", S_IRUSR | S_IWUSR);
   int fd1 = open("FIFOAB", O_RDWR);
   //int fd2 = open(FIFO_FILE2, O_CREAT|O_WRONLY);
   strcpy(end_str, "end");
   while(1){
      printf("Enter string: ");
      fgets(readbuf, sizeof(readbuf), stdin);
      stringlen = strlen(readbuf);
      readbuf[stringlen - 1] = '\0';
      end_process = strcmp(readbuf, end_str);
      
      //printf("end_process is %d\n", end_process);
      if (end_process != 0) {
         write(fd1, readbuf, strlen(readbuf));
         printf("CLIENT_A:Sent string: \"%s\" and string length is %d\n\n", readbuf, (int)strlen(readbuf));
      } else {
         write(fd1, readbuf, strlen(readbuf));
         printf("CLIENT_A:Sent string: \"%s\" and string length is %d\n\n", readbuf, (int)strlen(readbuf));
         close(fd1);
         break;
      }
   }
   return 0;
}


/*
int main()
{
   mkfifo("fifo1", S_IRUSR | S_IWUSR);
   int fd= open("fifo1", O_WRONLY);
   write(fd,"Passed msg\n",sizeof("Passed msg\n"));
   printf("Sending...");
   close(fd);
   return 0;
}
*/
