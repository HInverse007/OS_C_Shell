#include "functions.h"
#include "stdio.h"
#include "stdlib.h"
#include "errno.h"
#include "dirent.h"
#include "sys/stat.h"
#include "sys/types.h"
#include "string.h"
#include "signal.h"
#include "fcntl.h"
#include "unistd.h"
#include "sys/wait.h"

void hand(int sig)
{
	signal(SIGCHLD, handler);
}

int shell_setenv(char **args)
{
	int i = 0;
	while(args[i]!=NULL)
	{
		i++;
	}
	if(i == 2)
	{
		if(setenv(args[1]," ",2)!=0)
		{
			perror("my_shell");
		}
	}
	else if(i == 3)
	{
		if(setenv(args[1],args[2],2)!=0)
		{
			perror("my_shell");
		}
	}
	else
	{
		fprintf(stderr, "my_shell: setenv has wrong number of arguments.\n");
	}
	return 1;
}

int shell_unsetenv(char **args)
{
	int i =0;
	while(args[i]!=NULL)
	{
		i++;
	}
	if(i < 2)
	{
		fprintf(stderr, "my_shell: unsetenv has wrong number of arguments.\n");
	}
	else
	{
		i = 1;
		while(args[i]!=NULL)
		{
			if(unsetenv(args[i])!=0)
			{
				perror("my_shell");
			}
			i++;
		}
	}
	return 1;
}

int shell_jobs()
{
	int i = 1,fd;
	char pid[20],dir[]="/proc/", content[3000], **stats;
	while(i<=bgprocesscount)
	{
		printf("[%d]\t",proc_arr[i].number);
		sprintf(pid,"%d",proc_arr[i].id);
		strcpy(dir,"/proc/");
		strcat(dir, pid);
		strcat(dir, "/stat");
		fd = open(dir, O_RDONLY);
		if(fd == -1)
		{
			perror("my_shell");
			return 1;
		}
		read(fd, content, 2500);
		close(fd);
		stats = shell_args(content, " \t\r\n\a");
		if(stats[2][0]!='R' && stats[2][0]!='S')
		{
			printf("Stopped\t");
		}
		else
		{
			printf("Running\t");
		}
		printf("%s [%d]\n",proc_arr[i].name,proc_arr[i].id);
		i++;
	}
	return 1;
}

int shell_kjob(char **args)
{
	int i = 0,id,sig;
	while(args[i]!=NULL)
	{
		i++;
	}
	if(i != 3)
	{
		fprintf(stderr, "Syntax error.Usage: e.g.- kjob 2 9\n");
		return 1;
	}
	else
	{
		id = atoi(args[1]);
		sig = atoi(args[2]);
		kill(proc_arr[id].id,sig);
	}
	return 1;
}

int shell_overkill(char **args)
{
	int i = 1;
	while(i<=bgprocesscount)
	{
		kill(proc_arr[i].id,9);
		i++;
	}
	return 1;
}

int shell_fg(char **args)
{
	int i = 0,id,sig,status;
	pid_t wpid;
	proc temp;
	while(args[i]!=NULL)
	{
		i++;
	}
	if(i != 2)
	{
		fprintf(stderr, "Syntax error.Usage: e.g.- fg 2\n");
		return 1;
	}
	if(atoi(args[1]) == 0 || atoi(args[1]) > bgprocesscount)
	{
		fprintf(stderr, "The job number does not exist\n");
		return 1;
	}
	i=atoi(args[1]);
	temp = proc_arr[i];
	while(i<bgprocesscount)
	{
		proc_arr[i]=proc_arr[i+1];
		proc_arr[i].number--;
		i++;
	}
	bgprocesscount--;
	signal(SIGCHLD, hand);
	kill(temp.id,SIGCONT);
	/*
	tcsetpgrp(0, temp.id);
	waitpid(temp.id, &status, WUNTRACED);
	signal(SIGTTOU, SIG_IGN);
    tcsetpgrp(0, P);
    signal(SIGTTOU, SIG_DFL);*/
    do {
		wpid = waitpid(temp.id, &status, WUNTRACED);
	} while (!WIFEXITED(status) && !WIFSIGNALED(status) && !WIFSTOPPED(status));
	return 1;
}

int shell_bg(char **args)
{
	int i = 0,id,sig,fd,status,temp;
	while(args[i]!=NULL)
	{
		i++;
	}
	if(i != 2)
	{
		fprintf(stderr, "Syntax error.Usage: e.g.- bg 2\n");
		return 1;
	}
	if(atoi(args[1]) == 0 || atoi(args[1]) > bgprocesscount)
	{
		fprintf(stderr, "The job number does not exist\n");
		return 1;
	}
	kill(proc_arr[atoi(args[1])].id,SIGCONT);
	return 1;
}