#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>


float Temp=27.00;
bool isHighTemp=false;
pthread_mutex_t lock1=PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond2 = PTHREAD_COND_INITIALIZER;

void* lcdSensorReaderThread(void* arg)
{
	char *thead_id=(char *)arg;
	while(1) {
		pthread_mutex_lock(&lock1);
		pthread_cond_wait( &cond,&lock1);
		printf("%s:Current Temp:%f \n",thead_id,Temp);
		pthread_mutex_unlock(&lock1);
		sleep(1);		
	}
	return NULL;
}

void* SmartHomeMonitor(void* arg)
{
	char *thead_id=(char *)arg;
	while(1) {
		
		pthread_mutex_lock(&lock1);
		pthread_cond_wait( &cond,&lock1);
		//printf("%s:Current Temp:%f \n",thead_id,Temp);
		if(Temp<=45.00 && Temp>=27.00) {
			isHighTemp=false;	
			printf("%s:Temp:%f under control..!\n\n",thead_id,Temp);
		}
		else {
			printf("%s:Temp:%f out of control.\n\n",thead_id,Temp);
			
			isHighTemp=true;
			pthread_cond_signal( &cond2 ); 
			pthread_mutex_unlock(&lock1); // unlocking before exiting..
			pthread_exit(NULL);
		}		
		pthread_mutex_unlock(&lock1);
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
			pthread_mutex_lock(&lock1);
			printf("%s Locked Updating Temp\n\n",thead_id);
			Temp+=5.00;
			pthread_cond_signal( &cond );
			pthread_mutex_unlock(&lock1);
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

		pthread_mutex_lock(&lock1);
		
		while(1) {
		pthread_cond_wait( &cond2,&lock1); 
		if(isHighTemp==true) {
		
			printf("main:System Running:Red\n");
			pthread_cancel(tid1);
			pthread_cancel(tid2);
			break;
		}				
	}
	pthread_mutex_unlock(&lock1);
	printf("main:SmartHomeSystem Ending..!\n");	
	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);
	
	return 0;
}