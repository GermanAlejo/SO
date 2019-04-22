#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <semaphore.h>
#include <pthread.h>
//#include <sched.h>
//sched_yield(); // into osx we nust use this instead of pthread_yield

#define TAMBUF 8 // circular buffer size
#define NUMDATOS 100 // amount of data to send

// Circular buffer and pointers
int buffer[TAMBUF];
int bufin = 0;
int bufout = 0;
// Binary semaphore
pthread_mutex_t buffer_lock = PTHREAD_MUTEX_INITIALIZER;
// Adding var
unsigned int sum = 0;
// Semaforos generales
sem_t hay_datos;
sem_t hay_sitio;
// Circular buffer Writing & Reading functions
void obten_dato(int *itemp){
    *itemp = buffer[bufout];
    printf("Consumer reads %d on position %d \n",*itemp,bufout);
    bufout = (bufout + 1) % TAMBUF;
    return;
}

void pon_dato(int item) {
    printf("Producer puts %d on position %d \n",item,bufin);
    buffer[bufin] = item;
    bufin = (bufin + 1) % TAMBUF;
    return;
}

// Producer-consumer functions
void *productor(void *arg1) {
    int i;
    for (i = 1; i <= NUMDATOS; i++) {
        sem_wait(&hay_sitio);
        pthread_mutex_lock(&buffer_lock);
        pon_dato(i*i);
        pthread_mutex_unlock(&buffer_lock);
        sem_post(&hay_datos);
    }
    pthread_exit( NULL );
}

void *consumidor(void *arg2) {
    int i, midato;
    for (i = 1; i<= NUMDATOS; i++) {
        sem_wait(&hay_datos);
        pthread_mutex_lock(&buffer_lock);
        obten_dato(&midato);
        pthread_mutex_unlock(&buffer_lock);
        sem_post(&hay_sitio);
        sum += midato; 
    }
    pthread_exit( NULL );
}

// Main function
main() {
    pthread_t tidprod, tidcons;
    unsigned int i, total;
    total = 0;
    for (i = 1; i <= NUMDATOS; i++)
        total += i*i;
        // Semaphores initialization
        sem_init(&hay_datos, 0, 0);
        sem_init(&hay_sitio, 0, TAMBUF);
        // Thread creation
        pthread_create(&tidcons, NULL, consumidor, NULL);
        pthread_create(&tidprod, NULL, productor, NULL);
        
        // Wait for threads finnish
        pthread_join(tidprod, NULL);
        pthread_join(tidcons, NULL);
        printf("\n Result must be %d\n", total);
        printf("Threads has produced value %d\n", sum);
}

/*
What happens if in the consumer function we comment on the statement sem_wait (& hay_datos) ;? What if we delete the
statement sem_post (& hay_sitio);?

What happens if the producer function clears the instruction pthread_mutex_unlock (& buffer_lock) ;?

What happens if we initialize the semaphore there is_site to 0?

In this case, when there is a context switch? Why?

Why do threads always run until they are blocked? How could that behavior be changed? Try inserting the pthread_yield ()
statement to see its effect

*/