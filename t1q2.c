#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

void * print_integer(void* arg)
{
    printf("Integer:10\n");
}

void* print_float(void* arg) 
{  
    printf("Float:10.000000\n");
}

void* print_char(void* arg) 
{
    char ch='a';
    printf("Character:%c\n",ch);       
}

int main()
{
    pthread_t tid1, tid2, tid3;

    pthread_create(&tid1,NULL,print_integer,NULL);
    pthread_create(&tid2,NULL,print_float,NULL);
    pthread_create(&tid3,NULL,print_char,NULL);

    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);
    pthread_join(tid3,NULL);
    return 0;
}
