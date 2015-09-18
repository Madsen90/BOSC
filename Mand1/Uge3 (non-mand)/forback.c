/* 

   Opgave 1

   forback.c

 */

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

#include "forback.h"

/* start the program specified by filename with the arguments in argv 
   in a new process and wait for termination */
int foregroundcmd(char *filename, char *argv[])
{
	pid_t pid = fork();

	if(pid == 0){
		printf("%s\n", "im am child process");
		execvp(filename,argv);
	}else{
		printf("I am the father/mother process, and I will wait for my child\n");
		int returnStatus;    
    	waitpid(pid, &returnStatus, 0);
    	if(returnStatus == 0){
    		printf("%s\n", "My child is successful, and I am happy to terminate");
    	}
    	else{
    		printf("%s\n", "My child was unsuccessful. I will terminate in grief");
    	}
	}

	return 0;
}

/* start the program specified by filename with the arguments in argv 
   in a new process */
int backgroundcmd(char *filename, char *argv[])
{
	pid_t pid = fork();

	if(pid == 0){
		printf("%s\n", "im am child process");
		execvp(filename,argv);
	}else{
		printf("I am the father/mother process, but I wil NOT wait for it\n");
	}

	return 0;
}

int main(int argc, char const *argv[])
{

	printf("Program starting\n");

	char *argv2[argc];

	int i = 1;
	while(i < argc){
		argv2[i-1] = (char*) argv[i];
		i++;
	}
	
	argv2[i-1] = NULL;
	
	backgroundcmd(argv2[0], argv2);
	foregroundcmd(argv2[0], argv2);
	printf("\n");
	return 0;
}
