#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>


#define NUM_THREADS 5

void *PrintHello(void *threadid)
{  
   int i = (int)threadid;
   printf("\n%d: Hello World!\n", i);
   pthread_exit(NULL);
}

int main (int argc, char *argv[])
{
   pthread_t threads[NUM_THREADS];
   int rc, t;	
   for(t=0; t<NUM_THREADS; t++){
      printf("Creating thread %d\n", t);
      rc = pthread_create(&threads[t], NULL, PrintHello, (void *)t);      
      if (rc){
         printf("ERROR; return code from pthread_create() is %d\n", rc);
         exit(-1);
      }
   }
   for(t=0; t<NUM_THREADS; t++){
	pthread_join(threads[t],NULL);
	printf("thread %d ended\n",t);
   }
   pthread_exit(NULL);
}
