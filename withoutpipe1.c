#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define SIZE 10
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t condfull = PTHREAD_COND_INITIALIZER;
pthread_mutex_t lock;
int arr[SIZE],rear=-1,front=-1;                            

void *T1(void *arg)
{
    while(1)
       {
        pthread_mutex_lock(&lock);  //lock
        if(front==-1 && rear==-1)
            { 
                front=rear=0;
                printf("T1.1:Enter the integer:");
                scanf("%d",&arr[rear]);
            }
        else if((rear+1)%SIZE==front)
            {
                printf("T1:Pipe is Full,wait to get empty....\n");
                //break;
                pthread_cond_signal( &condfull );
            }
        else{
                rear =(rear+1)%SIZE;
                printf("T1.2:Enter the integer:");
                scanf("%d",&arr[rear]);
            }
        pthread_mutex_unlock(&lock);  //unlock
        pthread_cond_signal( &cond );
    } 
    return NULL;
}

void *T2(void *arg)
{
       pthread_cond_wait( &cond, &lock); //lock
       printf("T2:Signal....\n");
      // pthread_mutex_lock(&lock);  //lock
        while(1)
        {
        if(front==-1 && rear==-1)  
           {
                printf("T2.1:Pipe is Empty,wait to input....\n");
                //break;
                sleep(1);
           }
        else if(front==rear)  
        {
            front=rear=-1;
            printf("T2.2:Pipe is Empty,wait to input....\n");
            //break;
        }
        else 
            {      
                pthread_cond_wait( &condfull, NULL);
                printf("T2:read data is:%d\n",arr[front]);
                front=(front+1)%SIZE;
            }
        pthread_mutex_unlock(&lock);  //unlock
        }
    return NULL;
}

int main ()
{
    pthread_t tid1,tid2;
    pthread_mutex_init(&lock,NULL);
    pthread_create(&tid1, NULL, &T1, NULL);
    pthread_create(&tid2, NULL, &T2, NULL);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_mutex_destroy(&lock);
    return 0;
}