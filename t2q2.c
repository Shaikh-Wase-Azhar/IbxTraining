#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>


float Temp=27.00;
bool isHighTemp=false;
pthread_rwlock_t lock = PTHREAD_RWLOCK_INITIALIZER;
pthread_mutex_t mlock=PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond2 = PTHREAD_COND_INITIALIZER;

void* lcdSensorReaderThread(void* arg)
{
	char *thead_id=(char *)arg;
	while(1) {
		pthread_rwlock_rdlock(&lock);
		pthread_cond_wait( &cond,&lock);
		printf("%s:Current Temp:%f \n",thead_id,Temp);
		pthread_rwlock_unlock(&lock);
		sleep(1);		
	}
	return NULL;
}

void* SmartHomeMonitor(void* arg)
{
	char *thead_id=(char *)arg;
	while(1) {
		
		pthread_rwlock_rdlock(&lock);
		pthread_cond_wait( &cond,&lock);
		//printf("%s:Current Temp:%f \n",thead_id,Temp);
		if(Temp<=45.00 && Temp>=27.00) {

			pthread_mutex_lock(&mlock);
			isHighTemp=false;
			pthread_mutex_unlock(&mlock);	
			printf("%s:Temp:%f under control..!\n\n",thead_id,Temp);
		}
		else {

			printf("%s:Temp:%f out of control.\n\n",thead_id,Temp);
			pthread_mutex_lock(&mlock);
			isHighTemp=true;
			pthread_cond_signal( &cond2 ); 
			pthread_mutex_unlock(&mlock); // unlocking before exiting..
			pthread_rwlock_unlock(&lock); // unlocking before exiting..
			pthread_exit(NULL);
		}		
		pthread_rwlock_unlock(&lock);
		sleep(1);
		
			
	}
	return NULL;
}

void* sensorEvent(void* arg)
{
	int Timer=0;
	char *thead_id=(char *)arg;
	while(1) {
		
		if(Timer>5) {
			Timer=0;
			printf("%s Timer event  Received..! \n\n",thead_id);
			pthread_rwlock_wrlock(&lock);
			printf("%s Locked Updating Temp\n\n",thead_id);
			Temp+=5.00;
			pthread_cond_signal( &cond );
			pthread_rwlock_unlock(&lock);
		}	
		Timer++;	
		sleep(1);			
	}
	return NULL;
}

int main(void)
{
	int error;
	pthread_t tid1,tid2,tid3;
	
	error = pthread_create(&tid1,NULL,&lcdSensorReaderThread,(void *) "thread1_LCD");
	if (error != 0)
		printf("\nThread can't be created :[%s]",strerror(error));

	error = pthread_create(&tid2,NULL,&sensorEvent, (void *) "thread2_SSR");
	if (error != 0)
		printf("\nThread can't be created :[%s]",strerror(error));

	error = pthread_create(&tid2,NULL,&SmartHomeMonitor, (void *) "thread3_SHM");
	if (error != 0)
		printf("\nThread can't be created :[%s]",strerror(error));


	pthread_mutex_lock(&mlock);
	while(1) {

		pthread_cond_wait( &cond2,&mlock); 
		if(isHighTemp==true) {
		
			printf("main:System Running:Red\n");
			pthread_cancel(tid1);
			pthread_cancel(tid2);
			break;
		}				
	}
	pthread_mutex_unlock(&mlock);
	printf("main:SmartHomeSystem Ending..!\n");	
	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);
	
	return 0;
}