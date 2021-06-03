#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int fd;
float Temp=27.00;
unsigned char buff[100];

pthread_t tid1,tid2,tid3;
pthread_rwlock_t lock = PTHREAD_RWLOCK_INITIALIZER;
pthread_mutex_t mlock=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void* lcdSensorReaderThread(void* arg)
{
	char *thread_id=(char *)arg;
	printf("%s reading data......\n",thread_id);
	while(1) {
		
		pthread_rwlock_rdlock(&lock);
		pthread_cond_wait( &cond,&lock);
		read(fd,buff,sizeof("hello...\n"));
		printf("%s:buff:%f\n",thread_id,Temp);
		
		pthread_rwlock_unlock(&lock);
		sleep(1);					
	}
	return NULL;
}

void* SmartHomeMonitor(void* arg)
{
	char *thread_id=(char *)arg;
	printf("%s reading data......\n",thread_id);
	while(1) {
		
		pthread_rwlock_rdlock(&lock);
		pthread_cond_wait( &cond,&lock);
		read(fd,buff,sizeof("hello...\n"));
		printf("%s:buff:%f\n",thread_id,Temp);
		pthread_rwlock_unlock(&lock); // unlocking before exiting..		
		sleep(1);		
	}
	return NULL;
}

void* sensorEvent(void* arg) // write
{
	int Timer=0;
	char *thread_id=(char *)arg;
	while(1) {
		
		if(Timer>3) {
			Timer=0;

			pthread_rwlock_wrlock(&lock);
			Temp+=5.00;
			printf("%s Locked Updating Temp..%f\n\n",thread_id,Temp);
			write(fd,"hello...\n",sizeof("hello...\n"));
			pthread_cond_broadcast( &cond );
			pthread_rwlock_unlock(&lock);
		}	
		Timer++;	
		sleep(1);		
	}
	return NULL;
}

int main(void)
{
	fd=open("sensorout.txt", O_RDWR | O_CREAT);
	
	int error;
	error = pthread_create(&tid2,NULL,&sensorEvent, (void *) "thread2_SSR");
	if (error != 0)
		printf("\nThread can't be created :[%s]",strerror(error));

	error = pthread_create(&tid1,NULL,&lcdSensorReaderThread,(void *) "thread1_LCD");
	if (error != 0)
		printf("\nThread can't be created :[%s]",strerror(error));

	error = pthread_create(&tid3,NULL,&SmartHomeMonitor, (void *) "thread3_SHM");
	if (error != 0)
		printf("\nThread can't be created :[%s]",strerror(error));

	close(fd);
	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);
	pthread_join(tid3, NULL);

	return 0;
}