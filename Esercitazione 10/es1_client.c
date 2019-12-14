#include "./es1_header.h"

int main()
{
	struct sockaddr_un indirizzo;
	indirizzo.sun_family = AF_UNIX;
	strncpy(indirizzo.sun_path, SOCKNAME, strlen(SOCKNAME)+1);

	int sfd = socket(AF_UNIX, SOCK_STREAM, 0);
	ControllaENO(sfd == -1, "[client] socket");

	printf("[client] In attesa del server...\n");
	int retval = -1;
	while(retval == -1)
	{
		errno=0;
		retval = connect(sfd, (struct sockaddr*) &indirizzo, sizeof(indirizzo));
		ControllaENO(errno != 0 && errno != ENOENT, "[client] connect");
		if(errno == ENOENT) sleep(2);
	}
	printf("[client] Connesso!\n");

	while(1)
	{
		char operazione[LEN];
		retval = scanf("%s", operazione);
		ControllaENO(retval == EOF, "[client] lettura operazione aritmetica");
		int lunghezza = strlen(operazione);

		ControllaENO(write(sfd, &lunghezza, sizeof(int)) == -1, "[client] scrittura numero caratteri");
		ControllaENO(write(sfd, &operazione, lunghezza*sizeof(char)) == -1, "[client] scrittura operazione");
		if(strncmp(operazione, "quit", lunghezza) == 0) break;

		ControllaENO(read(sfd, &lunghezza, sizeof(int)) == -1, "[client] lettura numero caratteri");
		ControllaENO(read(sfd, &operazione, lunghezza*sizeof(char)) == -1, "[client] lettura risultato");
		printf("Risultato: %s\n", operazione);
	}

	ControllaENO(close(sfd) == -1, "[client] chiusura socket connessione");

	return 0;
}
