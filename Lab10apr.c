#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
void *reader(void *threadid)
{
	int temp = *(int *)threadid;
	printf("Thread no:  %d ",temp);
	FILE *fptr;
	int i=0;
	char ch,result[100];
	fptr = fopen("Buffer.txt", "r");
	if (fptr == NULL)
    {
        printf("Cannot open file \n");
        exit(0);
    }
	printf("Buffer: \n" );
	printf("\n\n");
	ch = fgetc(fptr);
	while (ch != EOF)
    {
    	i=0;
    	while(ch!='\n')
		{
			result[i++]=ch;
			ch = fgetc(fptr);
		}
		result[i++]='\0';
		//printf("Buffer: \n" );
		printf("%s",result );
		ch = fgetc(fptr);
		printf("\t");
	}
	printf("\n\n---- Thread is being closed ----\n");
	fclose(fptr);
    pthread_exit(NULL);
}
void main()
{
	pthread_t thread1;
	int rc,temp=1;
	rc = pthread_create(&thread1,NULL,reader,(void *)&temp);
	if (rc)
    {
        printf("ERROR; return code from pthread_create() is %d\n", rc);
        exit(-1);
    }
    pthread_join(thread1, NULL);
    pthread_exit(NULL);
}
