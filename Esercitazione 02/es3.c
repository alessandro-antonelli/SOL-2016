#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[])
{
	if(argc<3)
	{
		printf("Il programma deve essere chiamato con almeno due argomenti!\n");
		return -1;
	}

	char* mode = "w";
	if(argc>=4)
	{
		if(argv[3][0] == 'a') mode = "a";
	}
	printf("Mode: %s\n", mode);

	FILE *in, *out;

	in = fopen(argv[1], "r");
	if(in == NULL)
	{
		perror("Apertura del file di input non riuscito");
		return -1;
	}

	out = fopen(argv[2], mode);
	if(out == NULL)
	{
		perror("Apertura del file di output non riuscito");
		return -1;
	}

	int len;
	char* stringa = malloc(1024*sizeof(char));

	while(!feof(in))
	{
		fscanf(in, "%s", stringa);
		len = strlen(stringa);
		printf("Stringa: %s\n", stringa);

		int i;
		for(i=len-1; i>=0; i--)
		{
			fprintf(out, "%c", stringa[i]);
		}
		fprintf(out, "\n");
	}

	free(stringa);
	fclose(in);
	fclose(out);

	return 0;
}
