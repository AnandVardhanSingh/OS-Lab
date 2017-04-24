/*
The producer consumer problem 
*/

#include<stdio.h>
#include<pthread.h>
#include<semaphore.h> //semafor is an integer number mutex is binary semaphore mutex is used for locks
#include<string.h>
#include<stdlib.h>
#define BUFFSIZE 10  //SEMAFORE SHOWS HOW MANY BUFFERS ARE FULL!! 
pthread_t ctid[10];
pthread_t ptid[5];

int buffer[BUFFSIZE]; //Global variable
pthread_mutex_t mutex;
sem_t full, empty;
int counter;

void remove_item()
{
	int item;
	if(counter>0)
	{
		item = buffer[--counter];// ekn peeche likha hai isliye counter -1 se remover karenge as producer mein ++ kar diye the
		printf("\nConsuemr consumed :- %d\n",item);
	}
	printBuff();
}
int printBuff()
{
	int j;
	printf("\nBUFFER\n");
	for(j=0;j<BUFFSIZE;j++)
	printf("%d - ",buffer[j]);
}

void insert_item()
{
	int item = rand()%100;
	if(counter<BUFFSIZE)
	{
		buffer[counter]=item;
		counter++;
		printf("\nProducer produced :- %d\n",item);
	}
	printBuff();
}
void initilize()
{
	pthread_mutex_init(&mutex,NULL);
	sem_init(&empty,0,BUFFSIZE);
	sem_init(&full,0,0);
	counter =0;
}


void *prod(void *arg) // producer thread
{
	while(1)
	{
	sem_wait(&empty); // predefined function empty full hum banayenge
	pthread_mutex_lock(&mutex);
	insert_item();//this is the critical section
	pthread_mutex_unlock(&mutex);
	sem_post(&full); // predefined function
	}
}

void *con(void *arg) //consumer thread
{

	while(1)
	{
		sem_wait(&full); // this is predefined function empty full hum banayenge
		pthread_mutex_lock(&mutex);
		remove_item();//this is the critical section
		pthread_mutex_unlock(&mutex);
		sem_post(&empty); // sem post is a predefined function
	}
}



int main()
{
	initilize();
	int i;
	int num_prod = 5; // number of producer
	int num_con = 10; // number of consumer

	for(i=0;i<num_prod;i++) // creation of number of threads
	{
		pthread_create(&ptid[i],NULL,prod,NULL);  // ptid is value of that address so & is used
	}
	for(i=0;i<num_con;i++)
	{
		pthread_create(&ctid[i],NULL,con,NULL);
	}
	
	
	return 0;
}

