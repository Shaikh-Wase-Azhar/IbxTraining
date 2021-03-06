#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>

#define READ 0
#define WRITE 1

int main() {
    int pfds[2];
    char s[20];
    char buff[20];
   
   if (pipe(pfds) == -1) {
      printf("Unable to create pipe\n");
      return 1;
   }

   pid_t id = fork();
   if(0!=id)
   {  //parent
    printf("parent:Enter the msg:");
    scanf("%s",s);  
    printf("\nparent:Writing msg in pipe: %s\n",s);
    write(pfds[WRITE], s, sizeof(s));   
   }
   else 
   { //child
    read(pfds[READ], buff, sizeof(buff));
    printf("child:Read msg from pipe: %s\n", buff);
   }
   return 0;
}
