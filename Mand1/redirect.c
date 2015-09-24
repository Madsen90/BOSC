/* 
   Opgave 2

   redirect.c
 */

#include <unistd.h> //for STDIN_FILENO
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

/* start the program specified by filename with the arguments in argv 
   in a new process that has its stdin redirected to infilename and
   wait for termination */
int redirect_stdincmd(char *infilename)
{
  /* manipulate the file descriptor of the child process */
  int fid = open(infilename, O_RDONLY);
  
  /* replace stdin of the child process with fid */
  close(STDIN_FILENO); 
  
  //dup, duplicates to lowest id, which should be STDIN_FILENO. dup2 seems more secure.
  dup2(fid, STDIN_FILENO);

  close(fid);

  return 0;
}

/* start the program specified by filename with the arguments in argv 
   in a new process that has its stdout redirected to outfilename and 
   wait for termination */
int redirect_stdoutcmd(char *outfilename)
{

  /* manipulate the file descriptor of the child process */
  int fid = open(outfilename, O_WRONLY | O_CREAT, 0666);
  
  /* replace stdin of the child process with fid */
  close(STDOUT_FILENO); 
  
  //dup, duplicates to lowest id, which should be STDIN_FILENO. dup2 seems more secure.
  dup2(fid, STDOUT_FILENO);

  close(fid);

  return 0;
}
