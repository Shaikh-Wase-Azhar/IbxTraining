#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

void * sum(void* arr)
{
    int *ar=(int*)arr;
    int s=0;

    for(int i=0;i<10;i++)
        s=s+ar[i];

    printf("Sum of array elements:%d\n",s);
}

int main()
{
    int a[10]={1,2,3,4,5,6,7,8,9,10},
        b[10]={11,12,13,14,15,16,17,18,19,20},
        c[10]={21,22,23,24,25,26,27,28,29,30},
        d[10]={31,32,33,34,35,36,37,38,39,40};

    pthread_t tid1,tid2,tid3,tid4;

    pthread_create(&tid1,NULL,sum,(void *)a);
    pthread_create(&tid2,NULL,sum,(void *)b);
    pthread_create(&tid3,NULL,sum,(void *)c);
    pthread_create(&tid4,NULL,sum,(void *)d);

    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);
    pthread_join(tid3,NULL);
    pthread_join(tid4,NULL);
    return 0;
}
