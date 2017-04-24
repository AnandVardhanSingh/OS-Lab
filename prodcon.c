#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <semaphore.h>
#define bufferSize 10


int buffer[bufferSize];

pthread_t ctid[10];
pthread_t ptid[5];

pthread_mutex_t mutex;
sem_t full,empty;
int counter;

void *prod(void *arg);
void *cons(void *arg);
void initialize();
void insert_item();
void delete_item();
void printBuff();

int main()
{
	initialize();
	int i,rv;
	for (i = 0;i < 5;i++)
	{
		rv = pthread_create(&ptid[i],NULL,prod,NULL);
		if (rv)
    	{
        	printf("ERROR; return code from pthread_create() is %d\n", rv);
        	exit(-1);
    	}
	}
	for (i = 0;i < 10;i++)
	{
		rv = pthread_create(&ctid[i],NULL,cons,NULL);
		if (rv)
    	{
        	printf("ERROR; return code from pthread_create() is %d\n", rv);
        	exit(-1);
    	}
	}
}

void *prod(void *arg)
{
	while(1)
	{
		sem_wait(&empty);
		pthread_mutex_lock(&mutex);
		insert_item();
		pthread_mutex_unlock(&mutex);
		sem_post(&full);
	}
}
void *cons(void *arg)
{
	while(1)
	{
		sem_wait(&full);
		pthread_mutex_lock(&mutex);
		delete_item();
		pthread_mutex_unlock(&mutex);
		sem_post(&empty);
	}	
}

void initialize()
{
	pthread_mutex_init(&mutex,NULL);
	sem_init(&empty,0,bufferSize);
	sem_init(&full,0,0);
	counter = 0;
}

void insert_item()
{
	int item= rand();
	if (counter<bufferSize)
	{
		buffer[counter]=item;
		counter++;
	}
	printBuff();
}

void delete_item()
{
	int item;
	if (counter>0)
	{
		item=buffer[--counter];
	}
	printBuff();
}

void printBuff()
{
	int i;
	printf("\nBuffer: \n");
	for (i=0;i<bufferSize;i++)
	{
		printf("%d,  ",buffer[i]);
	}
	printf("\n");
}