#include <stdio.h>
#include <stdlib.h>

#define CHECK_CALL_PTR(file,stringa)	\
	if((file)==NULL) { 		\
		perror(#stringa);	\
		exit(EXIT_FAILURE);	\
	}

int main(int argc, char* argv[])
{
	FILE* file;
	CHECK_CALL_PTR(file=fopen(argv[1], "r"), Aprendo il file richiesto);
}
