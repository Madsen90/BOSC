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

/*Helper to avoid dublication*/
int redirectAndExec(char *filename, char *argv[], int in, int out, int closeId){
	if(in != -1){
		redirect_stdincmd(in);
	}
	if(out != -1){
		redirect_stdoutcmd(out);
	}	
	if(closeId != -1){
		close(closeId);
	}	
	
	if(execvp(filename,argv) == -1){
		printf("Command not found\n");
		exit(1);
	}
	return 0;
}


/* start the program specified by filename with the arguments in argv 
   in a new process and wait for termination */
int foregroundcmd(char *filename, char *argv[], int in, int out, int closeId)
{
	if(strcmp("exit", filename) == 0)
	{
		return -1;
	}
	pid_t pid = fork();

	if(pid == 0){
<<<<<<< HEAD
		if(in != -1){
			redirect_stdincmd(in);
		}
		if(out != -1){
			redirect_stdoutcmd(out);
		}	
		if(closeId != -1){
			close(closeId);
		}
		if(execvp(filename,argv) == -1){
			printf("Command not found");
		}
=======
		redirectAndExec(filename, argv, in, out, closeId);
>>>>>>> a14ce106792895a204a37d8707297a49e0fd47c7
	}else{
		int returnStatus;  
    	waitpid(pid, &returnStatus, 0);
	}

	return 0;
}


/* start the program specified by filename with the arguments in argv 
   in a new process */
int backgroundcmd(char *filename, char *argv[], int in, int out, int closeId)
{
	pid_t pid = fork();
	if(pid == 0){
		redirectAndExec(filename, argv, in, out, closeId);
	}

	return 0;
}