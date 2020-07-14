#include "functions.h"
#include "sys/types.h"
#include "unistd.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "sys/stat.h"
#include "fcntl.h"

int shell_pinfo(char **args)
{
	char pid[20],dir[]="/proc/", content[3000],*exePath=malloc(4000 * sizeof(char)),symlinkPath[50],**stats;
	int val,fd;
	if(args[1]==NULL)
	{
		sprintf(pid, "%d", getpid());
	}
	else
	{
		strcpy(pid,args[1]);
	}
	strcat(dir, pid);
	strcpy(symlinkPath, dir);
	strcat(dir, "/stat");
	strcat(symlinkPath, "/exe");
	val = readlink(symlinkPath, exePath, 4000);
	if(val == -1)
	{
		strcpy(exePath, "broken link");
	}
	else
	{
		exePath[val] = '\0';
	}
	exePath=shell_dir(exePath);
	fd = open(dir, O_RDONLY);
	if(fd == -1)
	{
		perror("my_shell");
		return 1;
	}
	read(fd, content, 2500);
	stats = shell_args(content, " \t\r\n\a");
	printf("pid -- %s\nProcess Status -- %s\n\n- %s{Virtual Memory} \n\n- Executable Path -- %s\n\n", stats[0], stats[2], stats[23], exePath);
	return 1;
}