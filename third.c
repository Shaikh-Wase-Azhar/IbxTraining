#include<stdio.h>
#include <fcntl.h>              
#include <unistd.h>
#include<string.h>


int main(int argc, char const *argv[])
{
 
    pid_t id;
    int i;
    unsigned char rbuff[64];
    int fd;
  
    
    id = fork();
    if(id==0)//child
  {
     mkfifo("fifo", S_IRUSR | S_IWUSR);

    fd = open("fifo", O_RDONLY);
    
    printf("read a string from sender\n");
    read(fd,rbuff,sizeof(rbuff));
    printf("Recv'd from sender: %s\n", rbuff);
    close(fd);
    
}
    

   
    else//parent
    {
        
       
    mkfifo("fifo", S_IRUSR | S_IWUSR);
    fd = open("fifo",  O_WRONLY);
    write(fd,"swati", 6);
    close(fd);
    return 0;
}
    }