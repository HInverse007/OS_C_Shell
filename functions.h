
void shell_loop();
char *shell_read();
int shell_execute(char **args,int bg_process);
char **shell_args(char *input,char *delimeter);
int shell_cd(char **args);
char *shell_dir(char dir[]);
void shell_exit();
int shell_pwd();
int shell_echo(char **args);
int shell_ls(char **args,int bg_process);
int shell_launch(char **args,int bg_process);
int shell_pinfo(char **args);
int shell_remind(char **args);
int shell_clock(char **args);
int shell_setenv(char **args);
int shell_unsetenv(char **args);
int shell_jobs();
int shell_kjob(char **args);
int shell_overkill(char **args);
int shell_fg(char **args);
int shell_bg(char **args);
void sighandler(int sig_num);
void handler(int sig);

char cwd[200],HOME[200],*display_dir,user[200],system_name[200],current[200];

typedef struct Proc
{
	int id;
	char name[100];
	int number;
	int fg;
} proc;

proc proc_arr[1000];

int bgprocesscount,clockloop,P,PID,chhand,shitproc[1000],shitnum;