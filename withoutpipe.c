#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t lock;
int arr[10];

void *in(void *arg)
{
    for(int i=0; i<10; i++)
        {pthread_mutex_lock(&lock);  //lock
        printf("T1:Enter the integer:\n");
        scanf("%d",arr[i]);
        pthread_cond_signal( &cond ); 
        pthread_mutex_unlock(&lock);  //unlock
    }
    return NULL;
}


void *out(void *arg)
{
        pthread_cond_wait( &cond,&lock); 
        for(int i=0; i<10; i++)
        {
        pthread_mutex_lock(&lock);  //lock
        printf("T2:%dth integer is:%d\n",i,arr[i]);
        pthread_mutex_unlock(&lock);  //unlock
        }
    return NULL;
}

int main ()
{
    pthread_t tid1,tid2;
    pthread_mutex_init(&lock,NULL);
    pthread_create(&tid1, NULL, &in, NULL);
    pthread_create(&tid2, NULL, &out, NULL);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_mutex_destroy(&lock);
    return 0;
}

