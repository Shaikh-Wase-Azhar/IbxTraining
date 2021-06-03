#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<string.h>

int i;
char buff[100];

char *s[10]={   "Dr. APJ A",
                "bdul Kalam",
                " :DREAM",
                "is",
                "Something",
                "which",
                "Doesn't",
                "let",
                "You",
                "SLEEP"};

void * add_string(void * arg)
{
    char *sp = (char *)arg;
    strcat(buff,sp);
}

int main()
{
    for(i=0; i<10;i++)
    {
        pthread_t tid[i];
        pthread_create(&tid[i],NULL,add_string,(void*)s+i);
    }
  
  for(i=0; i<10;i++)
  {
      pthread_t tid[i];
    pthread_join(tid[i], NULL);  
  }
     puts(buff);
    return 0;
}