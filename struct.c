#include<stdio.h>
#include<pthread.h>
#include<string.h>

struct values
{
	int a;
	char name[20];
};

void *printmsg(void *arg)
{
	struct values *b1= (struct values *)arg;	
	printf("%d,  %s \n ",b1->a,b1->name);
	pthread_exit(NULL);
}
	
int main()
{
	struct values a1;
	pthread_t tid;
	a1.a=10;
	int rv;
	
	strcpy(a1.name,"ECE section");
	rv = pthread_create(&tid,NULL,printmsg,(void*)&a1);
	if (rv)
	{
		printf("The thread was not created error code %d",rv);
	}	
	pthread_exit(NULL);	
}

