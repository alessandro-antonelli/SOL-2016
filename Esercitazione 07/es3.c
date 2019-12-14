#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
	if(argc!=2) { printf("Specificare il numero di processi!\n"); return -1; }

	int pid, i;

	for(i=0; i < atoi(argv[1]); i++)
	{
		pid = fork();
		if(pid == -1) { perror("Fork"); return -1; }
		if(pid == 0) { return 0; }
	}

	printf("Processo %d: terminazione tra un minuto\n", getpid() );
	sleep(60);

	return 0;
}
