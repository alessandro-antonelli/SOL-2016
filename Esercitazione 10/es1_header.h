#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#define SOCKNAME "./mysock"
#define LEN 1024


#define Controlla(guardia, testoerrore)			\
if(guardia)						\
{							\
	fprintf(stderr, "Errore nel file %s alla linea %d: %s\n", __FILE__, __LINE__, testoerrore);						\
	exit(EXIT_FAILURE);				\
}


#define ControllaENO(guardia, testoerrore)		\
if(guardia)						\
{							\
	perror("Errore nel file "__FILE__": " testoerrore);						\
	exit(EXIT_FAILURE);				\
}
