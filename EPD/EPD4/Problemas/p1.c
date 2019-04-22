#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 5

int globalCount = 0;
int elements[NUM_THREADS];

void *addValue(void* thread_id){

	int aux = (int)thread_id;
	printf("\nThread working: %d", aux);
	elements[globalCount] = globalCount;
	globalCount++;
	
}

int main(int argc, char *argv[]) {
	
	pthread_t threads[NUM_THREADS];
	int i, e;
	
	for(i=0; i<NUM_THREADS; i++){
		
		elements[i] = 0;
		
	}
	
	
	printf("\nPrinting initial array");
	for(i=0; i<NUM_THREADS; i++){
		printf("\nPosition %d -> %d", i, elements[i]);
	}
	
	for(i=0; i<NUM_THREADS; i++){
		
		printf("\nCreating thread %d", i);
		e = pthread_create(&threads[i], NULL, addValue, (void *)i);
		if(e){
			perror("ERROR creating thread");
			exit(-1);
		}
		
	}
	
	
   for(i=0; i<NUM_THREADS; i++){
		pthread_join(threads[i],NULL);
		printf("thread %d ended\n",i);
   }
  // pthread_exit(NULL);
	
	printf("\nPrinting final array");
	for(i=0; i<NUM_THREADS; i++){
		printf("\nPosition %d -> %d", i, elements[i]);
	}
	
	
 
}
