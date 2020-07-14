my_shell: main.c shell_loop.c shell_read.c shell_args.c built_in.c display_dir.c shell_ls.c shell_launch.c shell_pinfo.c built_in2.c
	gcc -o my_shell main.c shell_loop.c shell_read.c shell_args.c built_in.c display_dir.c shell_ls.c shell_launch.c shell_pinfo.c built_in2.c -I .
