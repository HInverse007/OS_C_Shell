#include "functions.h"
#include "stdio.h"
#include "string.h"

char *shell_read()
{
	char *input = NULL;
	int t;
	size_t len = 0;
	t=getline(&input,&len,stdin);
	while(t == -1)
	{
		t=getline(&input,&len,stdin);
	}
	return input;
}