#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <unistd.h>
#include <sys/wait.h>

#include "forback.h"
#include "parser.h"


/*
typedef struct _cmd {
    char **cmd;
    struct _cmd *next;
} Cmd;

typedef struct _shellcmd { 
    Cmd  *the_cmds;
    char *rd_stdin;
    char *rd_stdout;
    char *rd_stderr;
    int  background;
} Shellcmd;*/


int main(int argc, char const *argv[])
{
	//Shellcmd* cmd2;

	Cmd cmd1;
	Cmd cmd2;

	char* s1 = "ls";
	char *s11[] = {
		s1, NULL
	};
	
	cmd1.cmd = s11;

	char* s2 = "wc";
	char* s23 = "-c";
	char *s22[] = {
		s2, s23, NULL
	};
	
	cmd2.cmd = s22;

	/*int fd[2];

	if(pipe(fd) < 0){
		exit(1); //Not able to create pipe
	}*/



	int fd[2];

	if(pipe(fd) < 0){
		exit(1); //Not able to create pipe
	}

	//int inId, outId;
	//outId = fd[1];
	//inId  = -1;

	int fid = open("hej", O_WRONLY);
	printf("%s\n", " - - - -");
	foregroundcmd(*cmd1.cmd, cmd1.cmd, -1, fd[1], fd[0]);	
	printf("%s\n", " - - - -");
	

	printf("%s\n", " - - - -");
	foregroundcmd(*cmd2.cmd, cmd2.cmd, fd[0], -1, fd[1]);



	return 0;
}