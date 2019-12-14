#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void tokenizer(char *stringa, FILE *out)
{
	char* pezzoStringa = strtok(stringa, " ");
	while(pezzoStringa != NULL)
	{
		fprintf(out, "%s\n", pezzoStringa);
		pezzoStringa = strtok(NULL, " ");
	}
}

int main(int argc, char* argv[])
{
	int i;
	for(i=1; i<argc; i++)
	{
		tokenizer(argv[i], stdout);
	}

	return 0;
}
