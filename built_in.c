#include "functions.h"
#include "string.h"
#include "unistd.h"
#include "stdio.h"
#include "stdlib.h"
#include "sys/wait.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "fcntl.h"
#include "signal.h"

int shell_cd(char **args)
{
	if(args[1] == NULL)
	{
		fprintf(stderr, "my_shell: invalid syntax\n");
	}
	else if(strcmp(args[1],"~")==0)
	{
		if(chdir(HOME)!=0)
		{
			perror("my_shell");
		}
	}
	else
	{
		if(chdir(args[1])!=0)
		{
			perror("my_shell");
		}
	}
	return 1;
}

void handle(int sig)
{
	clockloop = 1;
}

int shell_clock(char **args)
{
	int i = 0,fd1,fd2,increment,ref,point,status;
	char time_file[] = "/sys/class/rtc/rtc0/time",date_file[]="/sys/class/rtc/rtc0/date",date[200],time[200],temp[200],**stats;
	signal(SIGINT,handle);
	signal(SIGTSTP,handle);
	while(args[i]!='\0')
	{
		i++;
	}
	if(i == 1)
	{
		increment = 1;
	}
	else if(i == 3)
	{
		if(strcmp(args[1],"-t") == 0)
		{
			increment = atoi(args[2]);
		}
		else
		{
			fprintf(stderr, "my_shell: invalid syntax\n");
			return 1;
		}
	}
	else
	{
		fprintf(stderr, "my_shell: invalid syntax\n");
		return 1;
	}

	fd1 = open(time_file, O_RDONLY);
	if(fd1 == -1)
	{
		perror("my_shell");
		return 1;
	}
	fd2 = open(date_file, O_RDONLY);
	if(fd2 == -1)
	{	
		perror("my_shell");
		return 1;
	}
	read(fd1,time,150);
	read(fd2,date,150);
	date[10]='\0';
	time[8]='\0';
	strcpy(temp,time);
	stats = shell_args(time,":");
	ref = atoi(stats[2]);
	printf("%s %s\n\n",date,temp);
	close(fd1);
	close(fd2);
	free(stats);
	clockloop = 0;
	while(clockloop == 0)
	{
		fd1 = open(time_file, O_RDONLY);
		if(fd1 == -1)
		{
			perror("my_shell");
			return 1;
		}
		fd2 = open(date_file, O_RDONLY);
		if(fd2 == -1)
		{	
		perror("my_shell");
			return 1;
		}
		read(fd1,time,150);
		read(fd2,date,150);
		date[10]='\0';
		time[8]='\0';
		strcpy(temp,time);
		stats = shell_args(time,":");
		point = atoi(stats[2]);
		if((ref + increment) == point)
		{
			ref = point;
			if((ref + increment) >= 60)
			{
				ref -= 60;
			}
			printf("%s %s\n\n",date,temp);
		}
		close(fd1);
		close(fd2);
		free(stats);
	}
	return 1;
}

int shell_remind(char **args)
{
	int i = 0,j = 0,pid,status;
	while(args[i]!=NULL)
	{
		i++;
	}
	if(i != 3)
	{
		fprintf(stderr, "my_shell: remindme should have 2 arguments e.g.- remindme 20 \"Go to class.\"\n");
	}
	else
	{
		pid = fork();
		if(pid == 0)
		{
			i = atoi(args[1]);
			if(args[2][0] == '\'' || args[2][0] == '"')
			{
				args[2][0] = ' ';
				while(args[2][j]!='\0')
				{
					j++;
				}
				j--;
				args[2][j] = '\0';
			}
			sleep(i);
			printf("\nReminder: %s\n",args[2]);
			exit(1);
		}
		else
		{
			bgprocesscount++;
        	proc_arr[bgprocesscount].id = (int) pid;
        	proc_arr[bgprocesscount].number = bgprocesscount;
        	strcpy(proc_arr[bgprocesscount].name,args[0]);
			waitpid(pid, &status, WNOHANG);
		}
	}
	return 1;
}

void shell_exit()
{
	exit(0);
}

int shell_pwd()
{
	char *pwd_dir=NULL;
	pwd_dir = shell_dir(cwd);
	if(strcmp(pwd_dir,"~")==0)
	{
		strcpy(pwd_dir,HOME);
	}
	printf("%s\n",pwd_dir);
	return 1;
}

int shell_echo(char **args)
{
	int i = 1;
	while(args[i]!=NULL)
	{
		printf("%s ",args[i]);
		i++;
	}
	printf("\n");
	return 1;
}

int shell_execute(char **args,int bg_process)
{
	int i = 0,flag = 0;
	if(strcmp("cd",args[0])==0)
	{
		return shell_cd(args);
	}
	else if(strcmp("quit",args[0])==0)
	{
		shell_exit();
	}
	else if(strcmp("pwd",args[0])==0)
	{
		return shell_pwd();
	}
	else if(strcmp("echo",args[0])==0)
	{
		return shell_echo(args);
	}
	else if(strcmp("ls",args[0])==0 || strcmp("l",args[0])==0)
	{
		return shell_ls(args,bg_process);
	}
	else if(strcmp("pinfo",args[0]) == 0)
	{
		return shell_pinfo(args);
	}
	else if(strcmp("remindme",args[0]) == 0)
	{
		return shell_remind(args);
	}
	else if(strcmp("clock",args[0]) == 0)
	{
		return shell_clock(args);
	}
	else if(strcmp("setenv",args[0]) == 0)
	{
		return shell_setenv(args);
	}
	else if(strcmp("unsetenv",args[0]) == 0)
	{
		return shell_unsetenv(args);
	}
	else if(strcmp("jobs",args[0]) == 0)
	{
		return shell_jobs();
	}
	else if(strcmp("kjob",args[0]) == 0)
	{
		return shell_kjob(args);
	}
	else if(strcmp("overkill",args[0]) == 0)
	{
		return shell_overkill(args);
	}
	else if(strcmp("fg",args[0]) == 0)
	{
		return shell_fg(args);
	}
	else if(strcmp("bg",args[0]) == 0)
	{
		return shell_bg(args);
	}
	else
	{
		return shell_launch(args,bg_process);
	}
}