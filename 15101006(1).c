/*
WAP to read an input file (one character at a time) and write it in another output FILE
a. Use multiple producer to read from file
b. Use mutliple conssumers to work in the output
c. Use bounded buffer to pass data from producer to consumer
*/

#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <semaphore.h>
#define bufferSize 100


char buffer[bufferSize];

pthread_t ctid[10];
pthread_t ptid[5];

FILE *fpprod,*fpcons;

pthread_mutex_t mutex;
sem_t full,empty;
int counter,flag;

void *prod(void *arg);
void *cons(void *arg);
void initialize();
void insert_item();
void delete_item();
char switchBuffer();

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
  sleep(10);
  fclose(fpprod);
  fclose(fpcons);
}

void initialize()
{
  fpprod = fopen("input.txt","r");
  fpcons = fopen("output.txt","w");
  pthread_mutex_init(&mutex,NULL);
  sem_init(&empty,0,bufferSize);
  sem_init(&full,0,0);
  counter = 0;
  flag =0;
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
void insert_item()
{
  char ch;
  if (counter<bufferSize&&flag==0)
  {
    ch = fgetc(fpprod);
    if (ch==EOF)
    {
      flag =1;
      return;
    }
    buffer[counter++]=ch;
  }
}
void delete_item()
{
  char ch;
  if (counter>0)
  {
    ch = switchBuffer();
    fputc(ch,fpcons);
  }
}
char switchBuffer()
{
  int i;
  char ch = buffer[0];
  for (i =0 ;i<counter;i++)
  {
    buffer[i]=buffer[i+1];
  }
  counter--;
  return ch;
}
