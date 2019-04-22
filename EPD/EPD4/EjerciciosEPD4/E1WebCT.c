#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *foo(void *arg) {	
        printf("Hi, I am a thread\n");			
   }

 int main() {
     pthread_t tid;
     if (pthread_create( &tid, NULL, foo, NULL) != 0 )
        printf("error: thread not created\n");
     printf("Hi, I am the main thread\n");
	pthread_join(tid, NULL);
     exit(0);
 }
