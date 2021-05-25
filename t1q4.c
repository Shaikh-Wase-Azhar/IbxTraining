#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t lock;
int n=1;

void *even(void *arg)
{
    while(1){
        pthread_mutex_lock(&lock);  //lock
        if(n>10){
        pthread_mutex_unlock(&lock);  //unlock
        break;
        }
        if(n%2==0)
        printf("Even thread :%d\n",n++);
        pthread_mutex_unlock(&lock);  //unlock
    }
    return NULL;
}


void *odd(void *arg)
{
    while(1){
        pthread_mutex_lock(&lock);  //lock
        if (n>10){
        pthread_mutex_unlock(&lock);  //unlock
        break;
        }
        if(n%2!=0)
        printf("Odd thread :%d\n",n++);
        pthread_mutex_unlock(&lock);  //unlock
    }
    return NULL;
}

int main ()
{
    pthread_t tid1,tid2;
    pthread_mutex_init(&lock,NULL);
    pthread_create(&tid1, NULL, &even, NULL);
    pthread_create(&tid2, NULL, &odd, NULL);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_mutex_destroy(&lock);
    return 0;
}

