#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

void *MyThread(void *);

pthread_mutex_t mutex; /* mutex id */

int main(void){
    pthread_t idA, idB; /* ids of threads */
    if (pthread_mutex_init(&mutex, NULL) < 0) {
        perror("pthread_mutex_init");
        exit(1);
    }
    if (pthread_create(&idA, NULL, MyThread, (void *)"A") != 0) {
        perror("pthread_create");
        exit(1);
    }
    if (pthread_create(&idB, NULL, MyThread, (void *)"B") != 0) {
        perror("pthread_create");
        exit(1);
    }
    (void)pthread_join(idA, NULL);
    (void)pthread_join(idB, NULL);
    (void)pthread_mutex_destroy(&mutex);
}

int x = 0; /* global shared variable */
void *MyThread(void *arg){
    char *sbName;
    sbName = (char *)arg;
    IncrementX();
    printf("X = %d in Thread %s\n", x, sbName);
}
IncrementX(){
    int Temp; /* local variable */
    BeginRegion(); /* enter critical region */
    Temp = x;
    Temp = Temp + 1;
    x = Temp;
    EndRegion(); /* exit critical region */
}

BeginRegion(){
    pthread_mutex_lock(&mutex);
}
EndRegion(){
    pthread_mutex_unlock(&mutex);
}

/*
What happens if in IncrementoX () we comment on the line EndRegion ();?
 
What could happen if in IncrementX () we comment on the BeginRegion () line?
 
*/