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
#include "redirect.h"

/* start the program specified by filename with the arguments in argv 
   in a new process and wait for termination */
int foregroundcmd(char *filename, char *argv[], int in, int out)
{
	pid_t pid = fork();

	if(pid == 0){
		if(in != -1){
			redirect_stdincmd(in);
		}
		if(out != -1){
			redirect_stdoutcmd(out);
		}	
		if(execvp(filename,argv) == -1){
			printf("Command not found");
		}
	}else{
		int returnStatus;    
    	waitpid(pid, &returnStatus, 0);

    	if(returnStatus == 0){
    	}
    	else{
    	}
	}

	return 0;
}

/* start the program specified by filename with the arguments in argv 
   in a new process */
int backgroundcmd(char *filename, char *argv[], int in, int out)
{
	pid_t pid = fork();
	if(pid == 0){
		if(in != -1){
			redirect_stdincmd(in);
		}
		if(out != -1){
			redirect_stdoutcmd(out);
		}	
		if(execvp(filename,argv) == -1){
			printf("Command not found");
		}
	}

	return 0;
}
