#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

/*
#define FIFO_FILE1 "MYFIFO1"
int main() {
   int end_process;
   int stringlen;
   char readbuf[80];
   char end_str[5];
   printf("CLIENT_A: Send messages, infinitely, to end enter \"end\"\n");
   int fd = open(FIFO_FILE1, O_CREAT|O_WRONLY);
   strcpy(end_str, "end");
   
   while (1) {
      printf("Enter string: ");
      fgets(readbuf, sizeof(readbuf), stdin);
      stringlen = strlen(readbuf);
      readbuf[stringlen - 1] = '\0';
      end_process = strcmp(readbuf, end_str);
      
      //printf("end_process is %d\n", end_process);
      if (end_process != 0) {
         write(fd, readbuf, strlen(readbuf));
         printf("CLIENT_A:Sent string: \"%s\" and string length is %d\n", readbuf, (int)strlen(readbuf));
      } else {
         write(fd, readbuf, strlen(readbuf));
         printf("CLIENT_A:Sent string: \"%s\" and string length is %d\n", readbuf, (int)strlen(readbuf));
         close(fd);
         break;
      }
   }
   return 0;
}
*/


int main()
{
   mkfifo("fifo1", S_IRUSR | S_IWUSR);
   int fd= open("fifo1", O_WRONLY);
   write(fd,"Passed msg\n",sizeof("Passed msg\n"));
   close(fd);
   return 0;
}




