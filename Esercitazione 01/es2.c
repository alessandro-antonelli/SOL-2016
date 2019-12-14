#include <stdio.h>

int main(int argc, char* argv[], char* envp[])
{
	printf("Sono stati passati al programma %d argomenti. Segue la lista:\n", argc);
	int i = 0;
	while(argv[i] != NULL)
	{
		printf("Argomento numero %d: %s\n", i, argv[i]);
		i++;
	}

	printf("====================================================================================\n");
	printf("Segue ora la lista delle variabili di ambiente:\n");
	i = 0;
	while(envp[i] != NULL)
	{
		printf("Variabile di ambiente numero %d: %s\n", i, envp[i]);
		i++;
	}

	printf("Fine del programma\n");

	return 0;
}
