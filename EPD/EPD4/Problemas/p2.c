#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 3

void *loop(void* thread_id){

	int id = (int)thread_id;
	printf("\nThread %d executing loop...", id);
	int c;
	for(c = 0; c < 100; c++){
		printf("\nIteration %d", c);
	}
	
	printf("\nThread %d ended", id);
	
	pthread_exit(NULL);
}

		   
void *productArrays(void* thread_id){

	int id = (int)thread_id;
	printf("\nThread %d executing array product...", id);
	int array1[5] = {1, 1, 1, 1, 1};
	int array2[5] = {1, 2, 3, 4, 5};
	int res[5];
	int i, aux;
	for(i=0; i<5;i++){
		res[i] = array1[i] * array2[i];
	}
	
	printf("\nProduct finished result is: ");
	
	for(i=0; i<5; i++){
		printf("%d ", res[i]);
	}
	
	
	printf("\nThread %d ended", id);
	
	pthread_exit(NULL);
}
		   
void *readInput(void* thread_id){

	int id = (int)thread_id;
	char s[50];
	printf("\nThread %d reading input", id);
	
	printf("\nWrite something: ");
	scanf("%s", &s);
	printf("\nYou said: %s", s);
	printf("\nThread %d ended", id);
	pthread_exit(NULL);
}
		   
int main(int argc, char *argv[]) {
	
	pthread_t threads[NUM_THREADS];
	int i, e;
	
	printf("\nCreating threads..");
	
	for(i=0; i<NUM_THREADS; i++){
		
		printf("\nCreating thread %d", i);
		if(i==0){
			e = pthread_create(&threads[i], NULL, loop, (void *)i);
			if(e){
				perror("ERROR creating thread");
				exit(-1);
			}
		}else if(i==1){
			e = pthread_create(&threads[i], NULL, productArrays, (void *)i);
			if(e){
				perror("ERROR creating thread");
				exit(-1);
			}
		}else{
			e = pthread_create(&threads[i], NULL, readInput, (void *)i);
			if(e){
				perror("ERROR creating thread");
				exit(-1);
			}
		}
		
	}
	
	printf("\nThread creating finished\n");
	
   for(i=0; i<NUM_THREADS; i++){
		pthread_join(threads[i],NULL);
		printf("thread %d ended\n",i);
   }
	
 
}
