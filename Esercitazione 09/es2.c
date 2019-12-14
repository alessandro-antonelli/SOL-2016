#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define ITERAZ 20
#define FILOSOFI 30

pthread_mutex_t semaforo[FILOSOFI];

void* filosofo(void* param)
{
	int MyNumber = (int) param;
	printf("Filosofo %d si sveglia!\n", MyNumber);

	struct timespec *tempo = malloc (sizeof(struct timespec));
	unsigned int seme = time(NULL);

	int i, secondi, nanosecondi;
	for(i=0; i<ITERAZ; i++)
	{
		//MEDITA
		secondi = rand_r(&seme);
		nanosecondi = rand_r(&seme);
		while(secondi>2) secondi = secondi/2;
		while(nanosecondi>1000000000) nanosecondi = nanosecondi/1000;
		tempo->tv_sec = secondi;
		tempo-> tv_nsec = nanosecondi;
		nanosleep(tempo, NULL);

		//ACQUISISCI FORCHETTE
		printf("Filosofo %d in attesa delle forchette...\n", MyNumber);
		pthread_mutex_lock(&semaforo[MyNumber-1 % FILOSOFI]);
		pthread_mutex_lock(&semaforo[MyNumber]);

		//MANGIA
		printf("Il filosofo %d sta mangiando!\n", MyNumber);
		secondi = rand_r(&seme);
		nanosecondi = rand_r(&seme);
		while(secondi>2) secondi = secondi/2;
		while(nanosecondi>1000000000) nanosecondi = nanosecondi/1000;
		tempo->tv_sec = secondi;
		tempo-> tv_nsec = nanosecondi;
		nanosleep(tempo, NULL);
		printf("Il filosofo %d è sazio!\n", MyNumber);

		//RILASCIA FORCHETTE
		pthread_mutex_unlock(&semaforo[MyNumber]);
		pthread_mutex_unlock(&semaforo[MyNumber-1 % FILOSOFI]);
	}
	printf("Filosofo %d si ritira...\n", MyNumber);
	return NULL;
}

int main()
{
	int i;
	pthread_t id[FILOSOFI];
	for(i=0; i<FILOSOFI; i++) pthread_mutex_init(&semaforo[i], NULL);

	for(i=0; i<FILOSOFI; i++)
	{
		if(pthread_create(&id[i], NULL, filosofo, (void*) i) != 0)
		{ printf("Errore creazione thread\n"); return -1; }
	}

	for(i=0; i<FILOSOFI; i++)
	{
		if(pthread_join(id[i], NULL) != 0)
		{ printf("Errore attesa termine thread\n"); return -1; }
	}
	printf("Fine del pranzo\n");
	return 0;
}
