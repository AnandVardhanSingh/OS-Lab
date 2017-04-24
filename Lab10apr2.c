#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

pthread_mutex_t lock;
FILE *fptr;
void *reader1(void *threadid)
{
	while(1)
	{
		pthread_mutex_lock(&lock);
		int temp = *(int *)threadid;
		printf("Thread no:  %d ",temp);
		printf("Lock accquired by reader 1\n");

		int i=0;
		char ch,result[100];

		ch = fgetc(fptr);
		if (ch == EOF)
	  {
	    printf("No more strings left \n");
			break;
	  }
		printf("\n\n");
	    i=0;
	    while(ch!='\n')
			{
				result[i++]=ch;
				ch = fgetc(fptr);
			}
			result[i++]='\0';
			puts(result);
			printf("\n");
		pthread_mutex_unlock(&lock);
		printf("Lock released by reader 1\n");
  }
	pthread_exit(NULL);
}
void *reader2(void *threadid)
{
	while(1)
	{
		pthread_mutex_lock(&lock);
		int temp = *(int *)threadid;
		printf("Thread no:  %d ",temp);
		printf("Lock accquired by reader 2\n");

		int i=0;
		char ch,result[100];

		ch = fgetc(fptr);
		if (ch == EOF)
	  {
	    printf("No more strings left \n");
			break;
	  }
		printf("\n\n");
	    i=0;
	    while(ch!='\n')
			{
				result[i++]=ch;
				ch = fgetc(fptr);
			}
			result[i++]='\0';
			puts(result);
			printf("\n");
		pthread_mutex_unlock(&lock);
		printf("Lock released by reader 2\n");
  }
	pthread_exit(NULL);
}
void main()
{
	pthread_t thread1[2];
	fptr = fopen("test.txt","r");
	int rc,temp1=1;
	rc = pthread_create(&thread1[0],NULL,reader1,(void *)&temp1);
	if (rc)
    {
        printf("ERROR; return code from pthread_create() is %d\n", rc);
        exit(-1);
    }
    int temp2 = 2;
    rc = pthread_create(&thread1[1],NULL,reader2,(void *)&temp2);
	if (rc)
    {
        printf("ERROR; return code from pthread_create() is %d\n", rc);
        exit(-1);
    }
    sleep(1);
		fclose(fptr);
    return;
}
