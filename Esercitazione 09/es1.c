#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

static pthread_mutex_t semaforo = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t condizione = PTHREAD_COND_INITIALIZER;
int buffer;
int vuoto=1;

void* consumatore(void* arg)
{
	while(1)
	{
		pthread_mutex_lock(&semaforo);
		while(vuoto)
			pthread_cond_wait(&condizione, &semaforo);
		printf("Ricevo %d\n", buffer);
		vuoto = 1;
		pthread_cond_signal(&condizione);
		pthread_mutex_unlock(&semaforo);
	}
	return 0;
}

int main()
{
	srand(time(NULL));
	pthread_t id;
	pthread_create(&id, NULL, consumatore, NULL);

	while(1)
	{
		pthread_mutex_lock(&semaforo);
		while(!vuoto)
			pthread_cond_wait(&condizione, &semaforo);
		buffer = rand();
		vuoto = 0;
		pthread_cond_signal(&condizione);
		pthread_mutex_unlock(&semaforo);
	}
	return 0;
}
