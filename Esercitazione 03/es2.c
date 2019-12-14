#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int ConfrontaMatrici(int (*funzione) (const void*, const void*, size_t), float* M1, float* M2, int bytes)
{
	return funzione(M1, M2, bytes);
}

int main(int argc, char*argv[])
{
	char* LOL;
	int size = (int) strtol(argv[1], &LOL, 10);
	float* M1;
	M1 = malloc(size*size*sizeof(float));

	//INIZIALIZZO
	int i;
	for(i=0; i<(size*size); i++)
	{
		M1[i] = i/2;
	}

	//DUMP
	FILE* dump;
	dump = fopen("mat_dump.dat", "wb");
	for(i=0; i<(size*size); i++)
	{
		fprintf(dump, "%f ", M1[i]);
	}
	fclose(dump);

	//LETTURA DUMP
	float* M2;
	M2 = malloc(size*size*sizeof(float));

	FILE* leggoDump;
	leggoDump = fopen("mat_dump.dat", "rb");
	for(i=0; i<(size*size); i++)
	{
		fscanf(leggoDump, "%f ", &M2[i]);
	}

	//CONFRONTO
	int risultato = ConfrontaMatrici(&memcmp, M1, M2, size*size*sizeof(float));
	if(risultato == 0) printf("Le matrici combaciano! 8)\n");
	else printf("Le matrici non combaciano... :(\n");

	free(M1);
	free(M2);
	return risultato;
}
