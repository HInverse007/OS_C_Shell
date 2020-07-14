#include "functions.h"
#include "string.h"
#include "unistd.h"
#include "stdio.h"
#include "stdlib.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "fcntl.h"
#include "sys/wait.h"
// ctrl-z
// update readme.md
#define COLOR_RED    "\x1b[31m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_RESET   "\x1b[0m"

void sighandler(int sig_num) 
{ 
    // Reset handler to catch SIGTSTP next time 
    signal(SIGTSTP, sighandler);
    signal(SIGINT, sighandler); 
} 

void shell_loop()
{
	int status = 1,i = 0,j,k,l,t,n,bg_process,flag, fd, fout, fin,stat,numPipes,redirect,kill;
	char *input=NULL, **args=NULL, **nargs=NULL, **margs=NULL, **commands=NULL, *temp = NULL, **poop=NULL, *hemp = NULL;
	pid_t p;
	P = getpid();
	while(status == 1)
	{
		signal(SIGTSTP, sighandler);
		signal(SIGINT, sighandler);
		getcwd(cwd,190);
		if(i == 0)
		{
			strcpy(HOME, cwd);
			i = 1;
			cuserid(user);
			gethostname(system_name,190);
		}
		display_dir = shell_dir(cwd);
		printf(COLOR_BLUE "<%s@%s:%s>" COLOR_RED,user,system_name,display_dir);
		input = shell_read();

		commands = shell_args(input,";");
		k = 0;
		while(commands[k]!=NULL)
		{
			redirect = 0;
			fout = dup(1);
			fin = dup(0);
			if(temp!=NULL)
			{
				free(temp);
				temp=NULL;
			}
			temp = malloc(400 * sizeof(char));
			j = 0;
			while(commands[k][j]!='\0')
			{
				temp[j]=commands[k][j];
				j++;
			}
			temp[j]='\0';
			args = shell_args(temp," \t\n\a\r");
			if(args[0]==NULL)
			{
				k++;
				continue;
			}
			j = 0;
			while(args[j]!=NULL)
			{
				j++;
			}
			j--;
			bg_process = 0;
			if(args[j][strlen(args[j]) - 1]=='&')
			{
				bg_process = 1;
				if(strlen(args[j]) > 1)
				{
					args[j][strlen(args[j]) - 1] = '\0';
				}
				else if(strlen(args[j]) == 1)
				{
					args[j] = NULL;
				}
			}
			if(temp!=NULL)
			{
				free(temp);
				temp=NULL;
			}
			temp = malloc(400 * sizeof(char));
			j = 0;
			while(commands[k][j]!='\0')
			{
				temp[j]=commands[k][j];
				j++;
			}
			temp[j]='\0';
			args = shell_args(temp,">");
			j = 0;
			while(args[j]!=NULL)
			{
				j++;
			}
			if(j == 2)
			{
				margs = shell_args(args[1]," \n");
				n=0;
				while(margs[n]!=NULL)
				{
					n++;
				}
				if(n!=1)
				{
					fprintf(stderr, "Syntax error.\n");
					k++;
				}
			}
			if(j == 2)
			{
//				printf("yes\n");
				if(temp!=NULL)
				{
					free(temp);
					temp=NULL;
				}
				temp = malloc(400 * sizeof(char));
				j = 0;
				kill = 0;
				while(commands[k][j]!='\0' && kill == 0)
				{
					temp[j]=commands[k][j];
					if(commands[k][j]=='>' && commands[k][j+1]=='>')
					{
						if(redirect == 0)
						{
							redirect = 1;
						}
						else
						{
							kill = 1;
						}
					}
					j++;
				}
				if(kill == 1)
				{
					fprintf(stderr, "Syntax error.\n");
					k++;
					continue;
				}
				temp[j]='\0';
				args = shell_args(temp," \n>");
				j = 0;
				while(args[j]!=NULL)
				{
					j++;
				}
				j--;
//				printf("%d\n",j);
				if(strlen(args[j]) > 1 && args[j][strlen(args[j]) - 1] == '&')
				{
					args[j][strlen(args[j]) - 1] = '\0';
				}
				else if(strlen(args[j]) == 1 && args[j][strlen(args[j]) - 1] == '&')
				{
					args[j] = NULL;
					j--;
				}
				if(redirect == 0)
				{
					fd = open(args[j], O_WRONLY | O_CREAT | O_TRUNC , 0644);
				}
				else
				{
					fd = open(args[j], O_WRONLY | O_CREAT | O_APPEND , 0644);
				}
				close(1);
				dup2(fd, 1);
				close(fd);
			}
			else if(j > 2)
			{
				fprintf(stderr, "Syntax error.\n");
				k++;
				continue;
			}
			if(temp!=NULL)
			{
				free(temp);
				temp=NULL;
			}
			temp = malloc(400 * sizeof(char));
			j = 0;
			while(commands[k][j]!='\0' && commands[k][j]!='>' && commands[k][j]!='&')
			{
				temp[j]=commands[k][j];
				j++;
			}
			temp[j]='\0';
			poop = shell_args(temp,"|");
//			printf("no\n");
			j = 0;
			while(poop[j]!=NULL)
			{
				j++;
			}
			if(j > 1)
			{
				int pipefd[2*j];
				numPipes = j;
				for(l = 0;l < j; l++)
				{
					if(pipe(pipefd + l*2) < 0)
					{
            			perror("couldn't pipe");
            			exit(EXIT_FAILURE);
        			}
				}
				if(hemp!=NULL)
				{
					free(hemp);
					hemp=NULL;
				}
				hemp = malloc(400 * sizeof(char));
				j = 0;
				while(poop[0][j]!='\0')
				{
					hemp[j]=poop[0][j];
					j++;
				}
				hemp[j]='\0';
				nargs = shell_args(hemp,"<");
				j = 0;
				while(nargs[j]!=NULL)
				{
					j++;
				}
				if(j == 2)
				{
					margs = shell_args(nargs[1]," \n");
					n=0;
					while(margs[n]!=NULL)
					{
						n++;
					}
					if(n!=1)
					{
						fprintf(stderr, "Syntax error.\n");
						k++;
						continue;
					}
				}
				if(j == 2)
				{
					if(hemp!=NULL)
					{
						free(hemp);
						hemp=NULL;
					}
					hemp = malloc(400 * sizeof(char));
					j = 0;
					kill = 0;
					while(poop[0][j]!='\0')
					{
						hemp[j]=poop[0][j];
						if(poop[0][j]=='<' && poop[0][j+1]=='<')
						{
							kill = 1;
						}
						j++;
					}
					if(kill == 1)
					{
						fprintf(stderr, "Syntax error.\n");
						k++;
						continue;
					}
					hemp[j]='\0';
					nargs = shell_args(hemp," <");
					j = 0;
					while(nargs[j]!=NULL)
					{
						j++;
					}
					j--;
					fd = open(nargs[j],O_RDONLY, 0);
					close(0);
					dup2(fd, 0);
					close(fd);
					if(hemp!=NULL)
					{
						free(hemp);
						hemp=NULL;
					}
					hemp = malloc(400 * sizeof(char));
					j = 0;
					while(poop[0][j]!='\0' && poop[0][j]!='<')
					{
						hemp[j]=poop[0][j];
						j++;
					}
					hemp[j]='\0';
					poop[0]=hemp;
				}
				else if(j > 2)
				{
					fprintf(stderr, "Syntax error.\n");
					k++;
					continue;
				}
				j = 0;
				l = 0;
				while(poop[j]!=NULL)
				{
					p = fork();
					if(p==0)
					{
						if(poop[j+1]!=NULL)
						{
							if(dup2(pipefd[l + 1],1)<0)
							{
								perror("dup2");
                    			exit(EXIT_FAILURE);
							}
						}
						if(l!=0)
						{
							if(dup2(pipefd[l-2], 0) < 0)
							{
                    			perror(" dup2");
                    			exit(EXIT_FAILURE);
                    		}
						}
						for(t = 0; t < 2*numPipes; t++)
						{
                    		close(pipefd[t]);
                    	}
						args = shell_args(poop[j]," \t\n\a\r");
						flag = shell_execute(args,bg_process);
						return;
					}
					flag = 1;
					if(flag == 0)
					{
						status = 0;
					}
					j++;
					l+=2;
				}
				for(t = 0; t < 2 * numPipes; t++)
				{
        			close(pipefd[t]);
        		}
    			for(t = 0; t < numPipes + 1; t++)
    			{
        			wait(&stat);
        		}
			}
			else
			{
//				printf("yes\n");
				if(hemp!=NULL)
				{
					free(hemp);
					hemp=NULL;
				}
				hemp = malloc(400 * sizeof(char));
				j = 0;
				kill = 0;
				while(poop[0][j]!='\0')
				{
					hemp[j]=poop[0][j];
					if(poop[0][j]=='<' && poop[0][j+1]=='<')
					{
						kill = 1;
					}
					j++;
				}
				if(kill == 1)
				{
					fprintf(stderr, "Syntax error.\n");
					k++;
					continue;
				}
				hemp[j]='\0';
				nargs = shell_args(hemp,"<");
				j = 0;
				while(nargs[j]!=NULL)
				{
					j++;
				}
				if(j == 2)
				{
					margs = shell_args(nargs[1]," \n");
					n=0;
					while(margs[n]!=NULL)
					{
						n++;
					}
					if(n!=1)
					{
						fprintf(stderr, "Syntax error.\n");
						k++;
						continue;
					}
				}
				if(j == 2)
				{
					if(hemp!=NULL)
					{
						free(hemp);
						hemp=NULL;
					}
					hemp = malloc(400 * sizeof(char));
					j = 0;
					while(poop[0][j]!='\0')
					{
						hemp[j]=poop[0][j];
						j++;
					}
					hemp[j]='\0';
					nargs = shell_args(hemp," \n<");
					j = 0;
					while(nargs[j]!=NULL)
					{
						j++;
					}
					j--;
					fd = open(nargs[j],O_RDONLY,0);
					close(0);
					dup2(fd, 0);
					close(fd);
					if(hemp!=NULL)
					{
						free(hemp);
						hemp=NULL;
					}
					hemp = malloc(400 * sizeof(char));
					j = 0;
					while(poop[0][j]!='\0' && poop[0][j]!='<')
					{
						hemp[j]=poop[0][j];
						j++;
					}
					hemp[j]='\0';
					poop[0]=hemp;
				}
				else if(j > 2)
				{
					fprintf(stderr, "Syntax error.\n");
					k++;
					continue;
				}


				args = shell_args(poop[0]," \t\n\a\r");
				flag = shell_execute(args,bg_process);
				flag = 1;
				if(flag == 0)
				{
					status = 0;
				}
			}
			k++;
			close(1);
			close(0);
			dup2(fout , 1);
			dup2(fin, 0);
			close(fin);
			close(fout);
		}
	}
	return;
}