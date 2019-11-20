/*
	Víctor Manuel Cavero Gracia - DNI: 45355080T
	Iván Fernández Sánchez - DNI: 52902115E
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int my_system(int args, char* argc[])
{

	for(int i = 1; i < args; i++)
	{
		printf("Ejecutando comando: %s\n", argc[i]);
		system(argc[i]);
		printf("\n");
	}

	return 0;
}

int main(int argc, char* argv[])
{
	if (argc < 2){
		fprintf(stderr, "Usage: %s <command>\n", argv[0]);
		exit(1);
	}

	return my_system(argc, argv);
}



