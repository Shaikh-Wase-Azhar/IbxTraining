#include<stdio.h>
#include <fcntl.h>              
#include <unistd.h>
#include<string.h>


int main(int argc, char const *argv[])
{
 
    pid_t id;
    char writemsgs[100];
    char readmsgs[100];
    int fd;
  
    
    id = fork();
    mkfifo("fifo", S_IRUSR | S_IWUSR);

    while(1){
    if(id==0)//child
  {

    fd = open("fifo", O_RDONLY);
    read(fd,readmsgs,sizeof(readmsgs));
    printf("Recv'd from sender: %s\n", readmsgs);
    close(fd);
    
}
    
 else//parent
    {
    printf("Enter the Parent message:\n");
    fd = open("fifo",  O_WRONLY);
    fgets(writemsgs,sizeof(writemsgs),stdin);
    //printf("Enter the Parent message:\n");
    write(fd,writemsgs,sizeof(writemsgs));
    
    }
}
return 0;
}