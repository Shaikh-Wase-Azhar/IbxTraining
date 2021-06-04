#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<string.h>

int i=0;
char buff[100];

char s[10][10]={"Dr.APJ",  
                "AbdulKalam",
                ":DREAM",
                "is",
                "Something",
                "which",
                "Doesn't",
                "let",
                "You",
                "SLEEP"};

void * add_string(void * arg)
{
  
    int  sp = *(int*)arg;
   // printf("thread id:%d %s\n",sp,s+sp);
    ///strcat(buff,s[*sp]);
   
  sprintf(buff+10*sp,"%s",s+sp);  
  
  //printf("buff:%s\n",buff+10*sp);
   /* if(sp==0) 
    {memcpy(&buff[10*(sp)],&s[sp],10);}

    else if(sp==1) 
    {memcpy(&buff[10*(sp)],&s[sp],10);}
*/
}

int main()
{
    pthread_t tid[10];
    for(i=0; i<10;i++)
    {
        pthread_create(&tid[i],NULL,add_string,(void*)&i);
        sleep(1);
    }
  
  for(i=0; i<10;i++)
    pthread_join(tid[i], NULL);  
  
  for(int i=0;i<sizeof(buff);i++)
  printf("%c",buff[i]);
     //puts(buff);
    return 0;
}