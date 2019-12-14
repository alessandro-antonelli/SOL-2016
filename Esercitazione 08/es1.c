#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char* argv[])
{
	if(argc < 2)
	{
		printf("Inserire un comando\n");
		return -1;
	}

	printf("****************************************\n");
	printf("Comando eseguito: ");
	int i;
	for(i=1; i < argc; i++) printf("%s ", argv[i]);
	printf("\n****************************************\n");

	//CREO PIPE
	int pfd[2], ret;
	ret = pipe(pfd);
	if(ret == -1) { perror("pipe"); return -1; }

	errno = 0;
	int capacita = fpathconf(pfd[0], _PC_PIPE_BUF);
	if(capacita == -1) if(errno != 0) { perror("lettura capacita massima pipe"); return -1; }

	//CREO IL PROCESSO
	int pid;
	pid = fork();
	if(pid == -1) { perror ("fork"); return -1; }
	if(pid == 0)
	{
		//FIGLIO
		close (pfd[0]);
		dup2(pfd[1], 1);
		if(close(pfd[1]) == -1)
		{ perror("close dello stdout non riuscita\n"); return -1; }

		pid = execvp(argv[1], &argv[1]);
		if(pid == -1)
		{ perror("exec non riuscita\n"); return -1; }

		printf("Ma come? Ancora qui?!\n");
		return -1;
	}

	//PADRE
	close (pfd[1]);
	char* buffer = malloc(capacita);
	int retvalue, StatusPointer;

	//STAMPO OUTPUT
	printf("Risultato:\n");

	while(1)
	{
		if(capacita == -1) capacita = 100 * sizeof(char);
		retvalue = read(pfd[0], buffer, capacita);
		if(retvalue == -1)
		{ perror("Lettura dalla pipe non riuscita\n"); return -1; }
		printf("%s\n", buffer);

		StatusPointer = 0;
		retvalue = waitpid(pid, &StatusPointer, WNOHANG);
		if(retvalue == -1) { perror("waitpid\n"); return -1; }
		if(WIFEXITED(StatusPointer))
		{ retvalue = WEXITSTATUS(StatusPointer); break; }
	}

	//FINE
	close(pfd[0]);
	free(buffer);
	printf("---------------------------------------- [%d]\n", retvalue);
	return 0;
}
