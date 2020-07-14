#include "functions.h"
#include "sys/types.h"
#include "unistd.h"
#include "stdlib.h"
#include "sys/wait.h"
#include "stdio.h"
#include "string.h"
#include "sys/stat.h"
#include "fcntl.h"

void handler(int sig)
{
	pid_t pid;
    int i = 1;
	pid = wait(NULL);
	if(pid!=-1)
	{
        while(proc_arr[i].id != pid)
        {
            i++;
        }
		printf("\n%s with pid %d Done.\n",proc_arr[i].name,pid);
        while(i<bgprocesscount)
        {
            proc_arr[i]=proc_arr[i+1];
            proc_arr[i].number--;
            i++;
        }
        bgprocesscount--;
	}
}

void sighandl(int sig)
{
    kill(PID,SIGINT);
    signal(SIGINT, sighandl);
}

void sighandle(int sig)
{
    int pid,status;
    signal(SIGTSTP, sighandle);
    signal(SIGCHLD, handler);
    printf("%d %d\n",getpid(),PID);
    kill(PID,SIGTSTP);
    //pid = fork();
    //if (pid < 0)
    //{
      //  perror("my_shell");
    //}
    //else if (pid == 0)
   // {
      //  kill(PID,SIGCONT);
  //      return;
//    }
    bgprocesscount++;
    proc_arr[bgprocesscount].id = (int) PID;
    proc_arr[bgprocesscount].fg = 0;
    proc_arr[bgprocesscount].number = bgprocesscount;
    strcpy(proc_arr[bgprocesscount].name,current);
    printf("[%d] %d\n", bgprocesscount, PID);
    return;
}

int shell_launch(char **args,int bg_process)
{
	int pid,wpid,status;
	signal(SIGCHLD, handler);
	pid = fork();
    if (pid < 0)
    {
        perror("my_shell");
        return 1;
    }
	else if (pid == 0)
	{
    	if (execvp(args[0], args) == -1)
    	{
    		perror("my_shell");
    	}
    	exit(EXIT_FAILURE);
    }
    PID = pid;
    if (bg_process == 1)
    {
        bgprocesscount++;
        proc_arr[bgprocesscount].id = (int) pid;
        proc_arr[bgprocesscount].fg = 0;
        proc_arr[bgprocesscount].number = bgprocesscount;
        strcpy(proc_arr[bgprocesscount].name,args[0]);
    	printf("[%d] %d\n", bgprocesscount, pid);
    	waitpid(pid, &status, WNOHANG);
    }
    else
    {
        strcpy(current,args[0]);
        signal(SIGTSTP, sighandle);
        signal(SIGINT, sighandl);
    	waitpid(pid, &status, WUNTRACED);
    }
	return 1;
}