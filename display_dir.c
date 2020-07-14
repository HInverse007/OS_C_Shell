#include "functions.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"

char *shell_dir(char dir[])
{
	char *value = malloc(200* sizeof(char));
	int len = strlen(HOME),i,j,flag=0;
	if(strcmp(HOME, dir)==0)
	{
		strcpy(value,"~");
	}
	else
	{
		for(i=0;HOME[i]!='\0' && flag == 0;i++)
		{
			if(HOME[i]!=dir[i])
			{
				flag = 1;
			}
		}
		if(flag == 1)
		{
			strcpy(value,dir);
		}
		else
		{
			value[0]='~';
			for(i=1,j=len;dir[j]!='\0';i++,j++)
			{
				value[i]=dir[j];
			}
			value[i]='\0';
		}
	}
	return value;
}