#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int CreaFiglio(int i)
{
	if(i != 0)
	{
		int j;
		for(j=0; j<i; j++) putchar('-');
		printf("%d: creo un processo figlio\n", getpid() );
		return fork();
	}
	else
	{
		printf("%d: sono l'ultimo discendente\n", getpid() );
		return -1;
	}
}

int main(int argc, char* argv[])
{
	if(argc != 2)
	{ printf("Inserisci il numero di processi da creare!\n"); return -1; }

	int pid, i, j;

	for(i=atoi(argv[1]); i >= 0; i--)
	{
		pid = CreaFiglio(i);
		if (pid != 0) break;
	}

	if (pid > 0)
	if (waitpid(pid, NULL, 0) == -1) { perror("waitpid"); return -1; }

	for(j=0; j<i; j++) putchar('-');
	printf("%d: terminato con successo\n", getpid() );

	return 0;
}
