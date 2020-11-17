# OS C shell
Written by Harsh Sharma

## To run the shell, enter the following commands:
	1. make
	2. ./my_shell

MODULES:

1. main
This contains the driver program from which the program is executed.

2. shell_loop
This contains the infinite loop for the shell to run.

3. shell_read
This consists of the read function used continuously by shell_loop.

4. shell_args
This tokenizes the input.

5. display_dir
This converts the home directory to a '\~' and converts other directories relative to '\~'.

6. shell_ls
This implements ls command along with the flags -a and -l.

7. built_in
Implements built in commands cd, echo, pwd, clock, remindme, quit,  and calls ls or pinfo function.

8. shell_pinfo
Implements pinfo command.

9. shell_launch
Implements regular shell functions using exec.

10. built_in2
Implements built in commands jobs, kjob, fg, bg, setenv, unsetenv, overkill. 
