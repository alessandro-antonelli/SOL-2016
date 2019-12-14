#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define N 10 //thread consumatori - numero totale di messaggi che i produttori producono
#define M 5 //thread produttori
#define INSERIMENTI 200000
#define TERMINAZIONE -1

struct _nodo {
	int value;
	struct _nodo* prev;
	struct _nodo* next;
};

struct _coda {
	struct _nodo* head;
	struct _nodo* tail;
	int lunghezza;
	int contachilometri;
};

typedef struct _nodo nodo;
typedef struct _coda coda_t;

coda_t* coda;
pthread_mutex_t semaforo = PTHREAD_MUTEX_INITIALIZER;



void* Consumatore(void* param)
{
	while(1)
	{
		printf("Consumatore fermo al semaforo...\n");
		pthread_mutex_lock(&semaforo);

		while(coda->lunghezza == 0)
		{
			printf("Consumatore non ha trovato niente da leggere!\n");
			pthread_mutex_unlock(&semaforo);
			sleep(2);
			pthread_mutex_lock(&semaforo);
		}

		int messaggio = coda->head->value;
		if(messaggio == TERMINAZIONE) break;
		if(coda->lunghezza != 1) //Nella coda c'erano anche altri messaggi
		{
			coda->head->next->prev = NULL;
			nodo* daliberare = coda->head;
			coda->head = coda->head->next;
			free(daliberare);
		}
		else //Nella coda c'era solo l'elemento che ho letto
		{
			free(coda->head);
			coda->head = NULL;
			coda->tail = NULL;
		}
		coda->lunghezza = coda->lunghezza - 1;

		printf("Consumatore ha ricevuto: %d! Ora ce ne sono %d\n", messaggio, coda->lunghezza);
		pthread_mutex_unlock(&semaforo);
	}

	pthread_mutex_unlock(&semaforo);
	printf("+++ Consumatore terminato +++\n");
	return NULL;
}

void* Produttore(void* param)
{
	unsigned int seme = time(NULL);
	srand(seme);

	while(1)
	{
		printf("Produttore fermo al semaforo...\n");
		pthread_mutex_lock(&semaforo);
		if(coda->contachilometri >= INSERIMENTI) break;

		nodo* elem = malloc(sizeof(nodo));
		elem->value = rand_r(&seme);
		elem->next = NULL;

		if(coda->lunghezza != 0) //Ci sono altri messaggi già in coda
		{
			elem->prev = coda->tail;
			coda->tail->next = elem;
			coda->tail = elem;
		}
		else //Sono il primo: non c'erano altri messaggi già in coda
		{
			elem->prev = NULL;
			coda->head = elem;
			coda->tail = elem;
		}
		coda->lunghezza = coda->lunghezza + 1;
		coda->contachilometri = coda->contachilometri + 1;
		
		printf("Produttore ha inserito un messaggio! Ora ce ne sono %d\n", coda->lunghezza);

		pthread_mutex_unlock(&semaforo);
	}

	//Messaggio di terminazione
	  if(!(coda->lunghezza != 0 && coda->tail->value == TERMINAZIONE))
	  {
	nodo* elem = malloc(sizeof(nodo));
	elem->value = TERMINAZIONE;
	elem->next = NULL;

	if(coda->lunghezza != 0) //Ci sono altri messaggi già in coda
	{

		elem->prev = coda->tail;
		coda->tail->next = elem;
		coda->tail = elem;

	}
	else //Sono il primo: non c'erano altri messaggi già in coda
	{
		elem->prev = NULL;
		coda->head = elem;
		coda->tail = elem;
	}
	coda->lunghezza = coda->lunghezza + 1;
	  }
		
	pthread_mutex_unlock(&semaforo);

	printf("+++ Produttore terminato +++\n");
	return NULL;
}

int main()
{
	coda = malloc(sizeof(coda_t));
	coda->lunghezza = 0;
	coda->contachilometri = 0;
	coda->head = NULL;
	coda->tail = NULL;

	int i;
	pthread_t idProduttori[M], idConsumatori[N];
	for(i=0; i<M; i++) pthread_create(&idProduttori[i], NULL, Produttore, NULL);
	for(i=0; i<N; i++) pthread_create(&idConsumatori[i], NULL, Consumatore, NULL);

	for(i=0; i<M; i++) pthread_join(idProduttori[i], NULL);
	printf("++++++ Tutti i produttori sono terminati! ++++++\n");
	for(i=0; i<N; i++) pthread_join(idConsumatori[i], NULL);
	printf("++++++ Tutti i consumatori sono terminati! ++++++\n");

	free(coda->head);
	printf("%d elementi rimanevano ancora nella coda; in totale %d messaggi inseriti\n", coda->lunghezza, coda->contachilometri);
	free(coda);

	printf("++++++ FINE! :) ++++++\n");
	return 0;
}
