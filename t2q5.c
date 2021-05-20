//Sir's code...

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>


float Temp=27.00;
bool isHighTemp=false;
pthread_t tid1,tid2,tid3;
pthread_rwlock_t lock = PTHREAD_RWLOCK_INITIALIZER;
pthread_mutex_t mlock=PTHREAD_MUTEX_INITIALIZER;
void* lcdSensorReaderThread(void* arg)
{
	char *thead_id=(char *)arg;
	while(1) {
		
		pthread_rwlock_rdlock(&lock);
		printf("%s:Current Temp:%f \n",thead_id,Temp);
		pthread_rwlock_unlock(&lock);
		sleep(1);
		
		pthread_mutex_lock(&mlock);
		if(isHighTemp==true) {
			pthread_mutex_unlock(&mlock);
			pthread_exit(NULL);
		}
		else
			pthread_mutex_unlock(&mlock);	
		
			
	}
	return NULL;
}

void* SmartHomeMonitor(void* arg)
{
	char *thead_id=(char *)arg;
	while(1) {
		
		pthread_rwlock_rdlock(&lock);
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
			pthread_mutex_unlock(&mlock); // unlocking before exiting..
			pthread_rwlock_unlock(&lock); // unlocking before exiting..
			//pthread_cancel(tid1);
			//pthread_cancel(tid2);
			pthread_exit(NULL);
		}		
		pthread_rwlock_unlock(&lock);
		sleep(1);
		
			
	}
	return NULL;
}

void* sensorEvent(void* arg) // write
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
			pthread_rwlock_unlock(&lock);
		}	
		Timer++;	
		sleep(1);
		pthread_mutex_lock(&mlock);
		if(isHighTemp==true) {
			pthread_mutex_unlock(&mlock);	
			pthread_exit(NULL);
		}
		else
			pthread_mutex_unlock(&mlock);		
				
	}
	return NULL;
}

int main(void)
{

	printf("main:SmartHomeSystem started..!\n");
	int i = 0;
	int error;
	
	error = pthread_create(&tid1,NULL,&lcdSensorReaderThread,(void *) "thread1_LCD");
	if (error != 0)
		printf("\nThread can't be created :[%s]",strerror(error));

	error = pthread_create(&tid2,NULL,&sensorEvent, (void *) "thread2_SSR");
	if (error != 0)
		printf("\nThread can't be created :[%s]",strerror(error));

	error = pthread_create(&tid2,NULL,&SmartHomeMonitor, (void *) "thread3_SHM");
	if (error != 0)
		printf("\nThread can't be created :[%s]",strerror(error));

	while(1) {
		pthread_mutex_lock(&mlock);
		if(isHighTemp==true) {

			printf("System Found Temp out of control..!\n");
			pthread_mutex_unlock(&mlock);
			//pthread_cancel(tid1);
			//pthread_cancel(tid2);
			break;
		}
		else 
			pthread_mutex_unlock(&mlock);

		sleep(5);
		
	}
	printf("main:SmartHomeSystem Ending..!\n");	
	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);
	
	return 0;
}
