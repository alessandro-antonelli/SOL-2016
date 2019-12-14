#include "./es1_header.h"

void Cleanup()
{
	unlink(SOCKNAME);
}

int main()
{
	Cleanup();
	struct sockaddr_un indirizzo;
	indirizzo.sun_family = AF_UNIX;
	strncpy(indirizzo.sun_path, SOCKNAME, strlen(SOCKNAME)+1);

	int sfd;
	ControllaENO((sfd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1, "[server] socket");

	ControllaENO(bind(sfd, (struct sockaddr*) &indirizzo, sizeof(indirizzo)) == -1, "[server] bind");

	Controlla(atexit(Cleanup) != 0, "[server] registrazione funzione cleanup");
	ControllaENO(listen(sfd, SOMAXCONN) == -1, "[server] listen");

	while(1)
	{
		printf("[server] In attesa di connessioni...\n");
		int sfdConnessione;
		ControllaENO((sfdConnessione = accept(sfd, NULL, NULL)) == -1, "[server] accept");
		printf("[server] Connessione rilevata\n");

		int versobc[2];
		int versoserver[2];
		ControllaENO(pipe(versobc) == -1, "[server] creazione pipe versobc");
		ControllaENO(pipe(versoserver) == -1, "[server] creazione pipe versoserver");

		int pid = fork();
		ControllaENO(pid == -1, "[server] fork");

		if(pid == 0)
		{
			//FIGLIO
			ControllaENO(close(versobc[1]) == -1, "[server - processo figlio] chiusura pipe");
			ControllaENO(close(versoserver[0]) == -1, "[server - processo figlio] chiusura pipe");

			//allo stdin ci collego la pipe
			ControllaENO(dup2(versobc[0],0) == -1, "[server - processo figlio] prima dup2");
			//alla pipe ci collego lo stdout
			ControllaENO(dup2(1,versoserver[1]) == -1, "[server - processo figlio] seconda dup2");

			execlp("/usr/bin/bc", "bc", "-q", NULL);
			Controlla(1, "[server - processo figlio] exec non riuscita!");
		}
		else
		{
			//PADRE
			ControllaENO(close(versobc[0]) == -1, "[server] chiusura pipe");
			ControllaENO(close(versoserver[1]) == -1, "[server] chiusura pipe");

			while(1)
			{
				char operazione[LEN];
				int lunghezza;

				ControllaENO(read(sfdConnessione, &lunghezza, sizeof(int)) == -1, "[server] lettura numero caratteri");
				ControllaENO(read(sfdConnessione, &operazione, lunghezza*sizeof(char)) == -1, "[server] lettura risultato");

				ControllaENO(write(versobc[1], &operazione, sizeof(operazione)) == -1, "[server] write diretta a processo figlio bc");
				//ControllaENO(write(versobc[1], &lunghezza, sizeof(int)) == -1, "[server] scrittura numero caratteri");
				//ControllaENO(write(versobc[1], &operazione, lunghezza*sizeof(char)) == -1, "[server] scrittura operazione");
				if(strncmp(operazione, "quit", lunghezza) == 0) break;

				ControllaENO(read(versoserver[0], &operazione, LEN) == -1, "[server] read per leggere dal processo figlio bc");
				lunghezza = strlen(operazione);
				ControllaENO(write(sfdConnessione, &lunghezza, sizeof(int)) == -1, "[server] scrittura numero caratteri");
				ControllaENO(write(sfdConnessione, &operazione, lunghezza*sizeof(char)) == -1, "[server] scrittura risultato operazione");
			}
			ControllaENO(close(sfdConnessione) == -1, "[server] chiusura socket della connessione");

			int status;
			do
			{
				ControllaENO(waitpid(pid, &status, 0) == -1, "[server] waitpid");
			} while(!WIFEXITED(status));
			if(WEXITSTATUS(status) != 0) printf("[server] processo della calcolatrice terminato con errore!\n");
		}
	}

	ControllaENO(close(sfd) == -1, "[server] chiusura socket per accettare connessioni");
	return 0;
}
