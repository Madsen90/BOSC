/* 
   Opgave 3
   
   pipe.c
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

#include "pipe.h"

/* create a pipe between two new processes, executing the programs 
   specified by filename1 and filename2 with the arguments in argv1
   and argv2 and wait for termination */
int pipecmd(char *filename1, char *argv1[], char *filename2, char *argv2[])
{
	//Forker til at lave barneproces
	pid_t pid = fork();

	if(pid == 0){
		int fd[2]; //int for holding ids for pipefiles

		if(pipe(fd) < 0){
			exit(1); //Not able to create pipe
		}

		//Forker så der er en proces til at skrive og en til at læde
		pid_t pid2 = fork();

		if(pid2 == 0){
			close(fd[0]);			//Lukker for læse enden 
			close(STDOUT_FILENO); 	//Lukker for STDOUT

			//duplikere skriveenden over i STDOUT
			dup2(fd[1], STDOUT_FILENO);

			//close(fd[1]); tror jeg er unødvendigt
			execvp(filename1, argv1);
		}else{
			close(fd[1]);			//Lukker for skrive enden 
			close(STDIN_FILENO); 	//Lukker for STDOUT

			//duplikere læseenden over i STDOUT
			dup2(fd[0], STDIN_FILENO);

			int returnStatus;    
		    waitpid(pid2, &returnStatus, 0);

		    if(returnStatus == 0){ 	//Vi vil ikke pipe hvis der er sket en fejl
		    	execvp(filename2, argv2);	
		    }else{
		    	
		    }
		}
	}else{
		int returnStatus;    
		waitpid(pid, &returnStatus, 0);
		printf("\n DONE \n");
	}


	return 0;
}
