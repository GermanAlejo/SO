#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define NUMSEARCHENGINE 3	//maximun number of search engines
#define MAXIMUM_CONNECTIONS 5 //maximun number of simultaneous connections

// Search status
#define READY 0	//search not found
#define IN_PROGRESS 1	//search is in progress 
#define FOUND 2	//search found

typedef struct {
		int id; // search id
		int status; // search status (READY (not found), IN_PROGRESS, FOUND)
		int found_at; // thread 's id that solves the search
}Search;

//variables globales
int cont = MAXIMUM_CONNECTIONS;
Search search;

//variables globales compartidas por hilos
int engine_id = 0;
int resError = 0;//this variable saves returning values when creating threads or semaphores

void *usuario (); //function that controls the user creates the seeker thread
void *buscador (); //funcion that controls seeker and creates the engines threads
void *searchEngine (); //funcion done by each seeker thread
void semaphoreStatus (int status);//returns the status of the semaphore depending of the int received

//non-binary semaphore
sem_t sem; //Para el maximo de conexiones simultaneas.
//binary semaphores
pthread_mutex_t searchResMutex;//this semaphore controls all engines threads wait for the firts one to finish
pthread_mutex_t syncMutex;//this semaphore controls access to some global variables

int main () {
	
	printf ("==========================================================================================\nUPO EPS Sistemas Operativos 2019\nPrueba Evaluable 2: Alejo Dominguez, German y Sanz Guijarro, Miguel Angel\n==========================================================================================\n");		

	int *finalStatus = 0;
	
	srand(time(NULL)); //seed to radomize time
	
	search.id = -1; 
	search.status = 0;
	search.found_at = -1;
	semaphoreStatus (0);//print semaphore status
	sem_init(&sem, 0, MAXIMUM_CONNECTIONS); //(Ref al semaforo, con cuantos procesos se comparte, valor inicial del semáforo)
	//sleep(1);
	printf ("[COMMON]initSearch.\n[COMMON]id = %d, status = %d, found_at = %d\n", search.id, search.status, search.found_at);	
	printf("[MAIN]Search data initialized\n");
	printf("[MAIN]Initializing search result mutex......search result mutex initialized.\n");
	resError = pthread_mutex_init(&searchResMutex, NULL);
	resError = pthread_mutex_init(&syncMutex, NULL);
	
	printf("[MAIN]Starting userThread ......user started.\n");
	pthread_t thread_user; 
	resError = pthread_create( &thread_user, NULL, (void *) usuario, NULL);
	//(Ref al hilo, atributos NULL = por defecto, funcion que ejecuta el hilo, argumento de entrada a la funcion)

	pthread_join(thread_user, NULL); //wait for user
	printf("[MAIN]User finished.\n");
	//destroy semaphores
	sem_destroy(&sem);//non binary semaphore
	pthread_mutex_destroy(&searchResMutex);
	pthread_mutex_destroy(&syncMutex);
	printf("[MAIN]Application has finished.\n");
	printf("=========================================================================\n");
	
	return resError; 
}

void *usuario () {
	
	sem_wait(&sem);
	semaphoreStatus (1);//print status

	printf ("	[USER]Started.\n	[USER]Requesting search...\n");
	search.status = IN_PROGRESS;
	search.id = 1;
	search.found_at = 0;
	
	printf ("[COMMON]updateSearch: id: %d, status: %d, found_at: %d\n", search.id, search.status, search.found_at);
	
	printf("[MAIN]Starting seekerThread......seekerThread started.\n");
	pthread_t thread_seeker;
	resError = pthread_create( &thread_seeker, NULL, (void *) buscador, NULL);
	
	pthread_join(thread_seeker, NULL);//wait for seeker	
	printf("[MAIN]Seeker finished.\n");
	
	printf("\t[USER]Search with text 1 resolved on searchEngine %d with status %d. Search result mutex has locked.\n", search.found_at, search.status);
	printf("\t[USER]Unlocking searchDataMutex...\n");

	printf("\t[USER]Finished.\n");
	

	sem_post(&sem);
	semaphoreStatus (2);//print status
}

void *buscador () {
	
	sem_wait(&sem);
	
	//start search/seeker
	printf ("	[SEEKER]Started.\n");

	//new connection from search	
	semaphoreStatus (1);
	
	//create array storing threads(engines)
	pthread_t pthreads[NUMSEARCHENGINE];
	
	printf("\t[SEEKER]Unlocked search processing search number 1.\n");
	int i;//int var to control loop 
	
	//loop to create threads(engines)
	for (i=0; i < NUMSEARCHENGINE; i++){
		//printf("\t\t\tVUELTA %d %d", aux, i);

		resError = pthread_create(&pthreads[i], NULL, (void *)searchEngine, NULL);
		
		printf ("	[SEEKER]Starting searchEngine %d...... searchEngine_id %d started.\n", i +1, i +1);
		
	}
	printf ("	[SEEKER]All SearchEngine launched.\n	[SEEKER]Waiting for all search engines to finish.\n");
	
	//do the wait and join here
	for (i=0; i<NUMSEARCHENGINE; i++) {
		
		pthread_join(pthreads[i], NULL);
		printf("\t[SEEKER] Search engine %d done.\n", i+1);
	}
	
	sem_post(&sem);
	semaphoreStatus (2);//print status
	printf ("	[SEEKER]Finished.\n");
	
}

void *searchEngine () {

	sem_wait(&sem);	
	semaphoreStatus (1);//print status
	
	pthread_mutex_lock(&syncMutex);
	engine_id++;//increment engine_id(shared global var)
	int x = engine_id; //Saves the value of engine_id that belongs to each searchEngine
	printf("\t\t[SEARCH ENGINE %d] is searching.\n", x);
	pthread_mutex_unlock(&syncMutex);
	
	int time =  rand()%5;//generate random number
	//printf("\n\t\t\tTIME: %d\n", time);
	printf("\t\t[SEARCH ENGINE %d] The search engine must invest %d seconds.\n", x, time);
	sleep (time);
	
	//printf("\n\n\t\t\tSEARCHID: %d\n\n", search.id);
	//lock search mutex
	pthread_mutex_lock(&searchResMutex);//make sure the other threads wait for first one to finish
	
	if (search.status != FOUND) {
		printf("\t\t[SEARCH ENGINE %d] has found search id %d. Unlocked search result.\n", x, search.id);
		search.status = FOUND;
		search.found_at = x;
		search.id = 0;
		printf("[COMMON]updateSearch: id: %d, status: %d, found_at: %d.\n", search.id, search.status, search.found_at);
	}

	printf("\t\t[SEARCH ENGINE %d] has finished.\n", x);
	
	pthread_mutex_unlock(&searchResMutex);
	
	sem_post(&sem);
	semaphoreStatus(2);
	
}


void semaphoreStatus (int status) {

	if (status == 1) {
		cont--;
		printf ("[COMMON]getConnection: connections semaphore now: %d\n", cont);
	}
	else if (status == 2){
		cont++;
		printf ("[COMMON]getConnection: connections semaphore now: %d\n", cont);
	}
	else {
		printf ("[MAIN]Connections semaphore initialized: %d\n", cont);
	}

}
