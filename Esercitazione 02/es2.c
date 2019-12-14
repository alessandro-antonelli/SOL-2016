#include <stdio.h>
#include <string.h>

void tokenizer_r(char *stringa, FILE *out)
{
	char* dovesonorimasto;
	char* token = strtok_r(stringa, " ", &dovesonorimasto);
	while(token != NULL)
	{
		fprintf(out, "%s\n", token);
		token = strtok_r(NULL, " ", &dovesonorimasto);
	}
}

int main(int argc, char* argv[])
{
	int i;
	for(i=1; i<argc; i++)
	{
		tokenizer_r(argv[i], stdout);
	}

	return 0;
}
