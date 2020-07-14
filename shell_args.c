#include "functions.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

char **shell_args(char *input,char *delimeter)
{
	int i,size = 30;
	char **args = malloc(size * sizeof(char*)),*token = NULL;

	if(args == NULL)
	{
		fprintf(stderr, "Space allocation error.\n");
		exit(EXIT_FAILURE);
	}

	token = strtok(input,delimeter);
	for(i = 0;token != NULL;i++)
	{
		if(i >= size)
		{
			size += 30;
			args = realloc(args, size * sizeof(char*));

			if(args == NULL)
			{
				fprintf(stderr, "Space allocation error.\n");
				exit(EXIT_FAILURE);
			}
		}
		args[i]=token;
		token = strtok(NULL,delimeter);
	}
	if( i >= size)
	{
		size += 4;
		args = realloc(args, size * sizeof(char*));

		if(args == NULL)
		{
			fprintf(stderr, "Space allocation error.\n");
			exit(EXIT_FAILURE);
		}
	}
	args[i] = NULL;
	return args;
}