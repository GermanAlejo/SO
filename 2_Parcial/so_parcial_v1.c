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

//variables globales compartidas por hilos
int engine_id = 1;
int resError = 0;//this variable saves returning values when creating threads or semaphores

void *usuario (Search *search); 
void *buscador (Search *search);
void *searchEngine (Search * search);
void semaphoreStatus (int status);//returns the status of the semaphore depending of the int received

//non-binary semaphore
sem_t sem; //Para el maximo de conexiones simultaneas.
//binary semaphores
pthread_mutex_t syncMutex;
pthread_mutex_t dataMutex;
pthread_mutex_t searchResMutex;

int main () {
	
	printf ("==========================================================================================\nUPO EPS Sistemas Operativos 2019\nPrueba Evaluable 2: <APELLIDO1> <APELLIDO2>,<NOMBRE>\n==========================================================================================\n");	
	
	int *finalStatus = 0;
	
	srand(time(NULL));
	
	Search search;
	search.id = -1; 
	search.status = 0;
	search.found_at = -1;
	semaphoreStatus (0);//print semaphore status
	sem_init(&sem, 0, MAXIMUM_CONNECTIONS); //(Ref al semaforo, con cuantos procesos se comparte, valor inicial del semáforo)
	sleep(1);
	printf ("[COMMON]initSearch.\n[COMMON]id = %d, status = %d, found_at = %d\n", search.id, search.status, search.found_at);	
	printf("[MAIN]Search data initialized\n");
	sleep(1);
	printf("[MAIN]Initializing search data mutex......search data mutex initialized.\n");
	resError = pthread_mutex_init(&dataMutex, NULL);
	printf("[MAIN]Initializing search processing mutex......search processing mutex initialized.\n");
	resError = pthread_mutex_init(&syncMutex, NULL);
	printf("[MAIN]Initializing search result mutex......search result mutex initialized.\n");
	resError = pthread_mutex_init(&searchResMutex, NULL);
	
	
	printf("[MAIN]Starting seekerThread......seekerThread started.\n");
	pthread_t thread_seeker;
	resError = pthread_create( &thread_seeker, NULL, (void *) buscador, &search);
	sleep(1);
	
	printf("[MAIN]Starting userThread ......user started.\n");
	pthread_t thread_user; 
	resError = pthread_create( &thread_user, NULL, (void *) usuario, &search); //NULL esta puesto por poner algo, podría hacer falta cambiarlo.
	//(Ref al hilo, atributos NULL = por defecto, funcion que ejecuta el hilo, argumento de entrada a la funcion)
	//sleep(2);
	
	pthread_join(thread_seeker, NULL);//wait for seeker	
	printf("[MAIN]Seeker finished.\n");
//	printf("HOLA");
	pthread_join(thread_user, NULL); //wait for user
	printf("[MAIN]User finished.\n");
	
	//destroy semaphores
	sem_destroy(&sem);//non binary semaphore
	pthread_mutex_destroy(&syncMutex);
	printf("[MAIN]Search sinchronization mutex has been destroyed.\n");
	pthread_mutex_destroy(&dataMutex);
	pthread_mutex_destroy(&searchResMutex);
	printf("[MAIN]Search data mutex has been destroyed.\n");
	printf("[MAIN]Application has finished.\n");
	printf("=========================================================================\n");
	
	
	return resError; 
}

void *usuario (Search *search) {
	
	sem_wait(&sem); //Cada vez que un usario haga una busqueda -1 al semaforo. cuando vale 0 el usuario queda bloqueado hasta que los otros terminen.
	//sleep(1);
	semaphoreStatus (1);//print status
	
	printf ("	[USER]Started.\n	[USER]Requesting search...\n");
	search->status = IN_PROGRESS;
	search->id = 1;
	search->found_at = 0;
	
	printf ("[COMMON]updateSearch: id: %d, status: %d, found_at: %d\n", search->id, search->status, search->found_at);
	pthread_mutex_lock(&syncMutex);
	pthread_mutex_lock(&dataMutex);
	
	printf("\t[USER]Search with text %d resolved on searchEngine %d with status %d. Search result mutex has locked.\n", search->id, search->found_at, search->status);
	printf("\t[USER]Unlocking searchDataMutex...\n");
	
	pthread_mutex_unlock(&dataMutex);
	pthread_mutex_unlock(&syncMutex);
	printf("\t[USER]Finished.\n");

	sem_post(&sem); //+1 al semaforo, asi nunca habra mas de 5 usuarios a la vez.
	semaphoreStatus (2);//print status
}

void *buscador (Search *search) {
	
	sem_wait(&sem);
	pthread_mutex_lock(&dataMutex);
	
	while (search->found_at !=0){}
	
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
		
		resError = pthread_create(&pthreads[i], NULL, (void *)searchEngine, &search);
		
		printf ("	[SEEKER]Starting searchEngine %d...... searchEngine_id %d started.\n", i+1, i+1);
		
	}
	printf ("	[SEEKER]All SearchEngine launched.\n	[SEEKER]Waiting for all search engines to finish.\n");

	pthread_mutex_unlock(&dataMutex);
	pthread_mutex_lock(&syncMutex);
		
	//do the wait and join here
	for (i=0; i<NUMSEARCHENGINE; i++) {
		
		pthread_join(pthreads[i], NULL);
		printf("\t[SEEKER] Search engine %d done.\n", i+1);
	}
		
	pthread_mutex_unlock(&syncMutex);
	
	sleep(1);
	
	sem_post(&sem);
	semaphoreStatus (2);//print status
	printf ("	[SEEKER]Finished.\n");
	
}

void *searchEngine (Search *search) {

	sem_wait(&sem);
	//sleep(1);
	
	
	
	semaphoreStatus (1);//print status
	printf("\t\t[SEARCH ENGINE %d] is searching.\n", engine_id);
	
	int time =  rand()%5;
	sleep (time);

	printf("\n\t\t\tTIME: %d\n", time);
	
	printf("\t\t[SEARCH ENGINE %d] The search engine must invest %d seconds.\n", engine_id, time);
	
	//lock search mutex
	pthread_mutex_lock(&searchResMutex);
	
	if (search->status != FOUND) {
		search->status = FOUND;
		search->found_at = engine_id;
		search->id = 0;
		printf("[COMMON]updateSearch: id: %d, status: %d, found_at: %d.\n", 
			   search->id, search->status, search->found_at);
		printf("\t\t[SEARCH ENGINE %d] has found search id %d. Unlocked search result.\n", engine_id,search->id);
	}

	
	
	printf("\t\t[SEARCH ENGINE %d] has finished.\n", engine_id);
	engine_id++;//increment engine_id(shared global var)
		
	pthread_mutex_unlock(&searchResMutex);

	
	sem_post(&sem);
	//sleep(1);
	semaphoreStatus (2);
	
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