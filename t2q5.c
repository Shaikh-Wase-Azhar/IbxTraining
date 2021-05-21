//thread read-write lock....

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


float Temp=27.00;
int fd;
char buff[100];
//fd=open("data.txt", O_RDONLY | O_WRONLY);

pthread_t tid1,tid2,tid3;
pthread_rwlock_t lock = PTHREAD_RWLOCK_INITIALIZER;
pthread_mutex_t mlock=PTHREAD_MUTEX_INITIALIZER;

void* lcdSensorReaderThread(void* arg)
{
	char *thread_id=(char *)arg;
	printf("%s reading data:\n",thread_id);
	while(1) {
		
		pthread_rwlock_rdlock(&lock);
		read(fd,buff,5);
		//read(fd,buff,6);
		printf("%s.....buff:%s\n",thread_id,buff);
		
		pthread_rwlock_unlock(&lock);
		sleep(1);					
	}
	return NULL;
}

void* SmartHomeMonitor(void* arg)
{
	char *thread_id=(char *)arg;
	printf("%s reading data:\n",thread_id);
	while(1) {
		
		pthread_rwlock_rdlock(&lock);
		read(fd,buff,5);
		//read(fd,buff,6);
		printf("%s.....buff:%s\n",thread_id,buff);
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
		
		if(Timer>5) {
			Timer=0;

			pthread_rwlock_wrlock(&lock);
			printf("%s Locked Updating Temp..%f\n\n",thread_id,Temp);
			Temp+=5.00;
            write(fd,&Temp,5);
			//write(fd,"Hi...\n",6);
			pthread_rwlock_unlock(&lock);
		}	
		Timer++;	
		sleep(1);		
	}
	return NULL;
}

int main(void)
{
	fd=open("sensorout.txt", O_RDONLY | O_WRONLY);
	
	int error;	
	error = pthread_create(&tid1,NULL,&lcdSensorReaderThread,(void *) "thread1_LCD");
	if (error != 0)
		printf("\nThread can't be created :[%s]",strerror(error));

	error = pthread_create(&tid2,NULL,&sensorEvent, (void *) "thread2_SSR");
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
