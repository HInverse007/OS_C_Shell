#include "functions.h"
#include "string.h"
#include "stdio.h"
#include "dirent.h"
#include "stdlib.h"
#include "time.h"
#include "sys/stat.h"
#include "sys/types.h"
#include "malloc.h"
#include "pwd.h"
#include "grp.h"

int shell_ls(char **args,int bg_process)
{
	int flag_l = 0, flag_a = 0,i=0;

	if(args[1]==NULL)
	{
	}
	else if(strcmp(args[1],"-l")==0)
	{
		flag_l = 1;
		if(args[2]==NULL)
		{}
		else if(strcmp(args[2],"-a")==0)
		{
			flag_a = 1;
		}
		else
		{
			fprintf(stderr, "my_shell: invalid syntax\n");
			return 1;
		}
	}
	else if(strcmp(args[1],"-a")==0)
	{
		flag_a = 1;
		if(args[2]==NULL)
		{}
		else if(strcmp(args[2],"-l")==0)
		{
			flag_l = 1;
		}
		else
		{
			fprintf(stderr, "my_shell: invalid syntax\n");
			return 1;
		}
	}
	else if(strcmp(args[1],"-la")==0 || strcmp(args[1],"-al")==0)
	{
		flag_l = 1;
		flag_a = 1;
		if(args[2]==NULL)
		{}
		else
		{
			fprintf(stderr, "my_shell: invalid syntax\n");
			return 1;
		}
	}
	else
	{
		fprintf(stderr, "shell: invalid syntax\n");
		return 1;
	}
	if(flag_a == 1 && flag_l == 0)
	{
		struct dirent *de;   
    	DIR *dr = opendir(".");
    	while ((de = readdir(dr)) != NULL)
    	{
        	   printf("%s\n", de->d_name);
    	}
    	closedir(dr);
    }
    else if(flag_a == 0 && flag_l == 0) 
    {
    	struct dirent *de;   
    	DIR *dr = opendir(".");
    	while ((de = readdir(dr)) != NULL)
    	{
    		if(de->d_name[0]!='.')
    		{
        	    printf("%s\n", de->d_name);
    		}
    	}
    	closedir(dr);
    }   
    else if(flag_a == 1 && flag_l == 1)
    {
    	struct dirent *de;   
    	DIR *dr = opendir(".");
    	struct stat directory;
    	char date[100];
    	while ((de = readdir(dr)) != NULL)
    	{
    		stat(de->d_name, &directory);
	    	strftime(date, 100, "%h %d %H:%M", localtime(&(directory.st_ctime)));
	        printf( (S_ISDIR(directory.st_mode)) ? "d" : "-");
	        printf( (directory.st_mode & S_IRUSR) ? "r" : "-");
	        printf( (directory.st_mode & S_IWUSR) ? "w" : "-");
	        printf( (directory.st_mode & S_IXUSR) ? "x" : "-");
	        printf( (directory.st_mode & S_IRGRP) ? "r" : "-");
	        printf( (directory.st_mode & S_IWGRP) ? "w" : "-");
	        printf( (directory.st_mode & S_IXGRP) ? "x" : "-");
	        printf( (directory.st_mode & S_IROTH) ? "r" : "-");
	        printf( (directory.st_mode & S_IWOTH) ? "w" : "-");
	        printf( (directory.st_mode & S_IXOTH) ? "x" : "-");
	        printf(" %d ", (int)directory.st_nlink);
	        struct group  *gr = getgrgid(directory.st_gid);
	        struct passwd *pw = getpwuid(directory.st_uid);
	        printf(" %s", pw->pw_name);
	        printf(" %s", gr->gr_name);
	        printf(" %s     ", date);
	        printf("%d", (int)directory.st_size);
	        printf(" \t%s", de->d_name);
	        printf("\n");
	    }
	}
	else if(flag_a == 0 && flag_l == 1)
    {
    	struct dirent *de;   
    	DIR *dr = opendir(".");
    	struct stat directory;
    	char date[100];
    	while ((de = readdir(dr)) != NULL)
    	{
    		if(de->d_name[0]!='.')
    		{
	    		stat(de->d_name, &directory);
		    	strftime(date, 100, "%h %d %H:%M", localtime(&(directory.st_ctime)));
		        printf( (S_ISDIR(directory.st_mode)) ? "d" : "-");
		        printf( (directory.st_mode & S_IRUSR) ? "r" : "-");
		        printf( (directory.st_mode & S_IWUSR) ? "w" : "-");
		        printf( (directory.st_mode & S_IXUSR) ? "x" : "-");
		        printf( (directory.st_mode & S_IRGRP) ? "r" : "-");
		        printf( (directory.st_mode & S_IWGRP) ? "w" : "-");
		        printf( (directory.st_mode & S_IXGRP) ? "x" : "-");
		        printf( (directory.st_mode & S_IROTH) ? "r" : "-");
		        printf( (directory.st_mode & S_IWOTH) ? "w" : "-");
		        printf( (directory.st_mode & S_IXOTH) ? "x" : "-");
		        printf(" %d ", (int)directory.st_nlink);
		        struct group  *gr = getgrgid(directory.st_gid);
		        struct passwd *pw = getpwuid(directory.st_uid);
		        printf(" %s", pw->pw_name);
		        printf(" %s", gr->gr_name);
		        printf(" %s     ", date);
		        printf("%d", (int)directory.st_size);
		        printf(" \t%s", de->d_name);
		        printf("\n");
		    }
	    }
	}
	return 1;
}