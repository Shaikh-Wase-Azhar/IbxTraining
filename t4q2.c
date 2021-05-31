#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>

sem_t count_sem;
int *count;
int input;


void *sendr(void *arg)
{        
    while(1){
        sem_wait(&count_sem);
        scanf("%d",&input);
        *count=input;
        sem_post(&count_sem);
    }
    return NULL;
}


void *rcvr(void *arg)
{
    while(1){
        sem_wait(&count_sem);
        printf("count: %d",*count);
        sem_post(&count_sem);   
    }
    return NULL;
}

int main ()
{
    int shmfd;
    shmfd= shm_open("/shm_name", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

    ftruncate(shmfd, sizeof(int));
    count=(int *)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0);

    pthread_t tid1,tid2;
    
    sem_init(&count_sem,0,1);
    
    pthread_create(&tid1, NULL, &sendr, NULL);
    pthread_create(&tid2, NULL, &rcvr, NULL);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    
    sem_destroy(&count_sem);

    shm_unlink("/shm_name");

    return 0;
}
