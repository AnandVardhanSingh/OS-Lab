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


char buffer[bufferSize][bufferSize];

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
void strrev(char str[]);
void add_in_buffer(char str[]);

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
  sleep(2);
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
	if(feof(fpprod))
		return;
	int i;
	char ch,result[bufferSize];
	ch = fgetc(fpprod);
	i=0;
	while(ch!='\n'&&ch!=EOF)
	{
		result[i++]=ch;
		ch = fgetc(fpprod);
	}
	result[i++]='\0';
	strrev(result);
	add_in_buffer(result);
	counter++;
	return;
}
void delete_item()
{
	int i;
	char str[bufferSize];
  if (counter>0)
  {
		for (i = 0;i<bufferSize;i++)
		{
			str[i]=buffer[0][i];
		}
    switchBuffer();
    fputs(str,fpcons);
		fputc('\n',fpcons);
  }
}
char switchBuffer()
{
	int i,j;

	for (i =0 ;i<counter;i++)
  {
		for(j=0;j<bufferSize;j++)
		{
    	buffer[i][j]=buffer[i+1][j];
		}
	}
  counter--;
}
void strrev(char str[])
{
	char temp;
	int len = strlen(str);
	int i;
	for (i=0;i<len/2;i++)
	{
		temp = str[len-i-1];
		str[len -i -1]=str[i];
		str[i]= temp;
	}
}
void add_in_buffer(char str[])
{
	int i;
	for (i=0;i<bufferSize;i++)
	{
		buffer[counter][i]=str[i];
	}
}
