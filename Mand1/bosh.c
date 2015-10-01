 /* 
   bosh.c : BOSC shell 
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
<<<<<<< HEAD
#include <sys/types.h>
#include <unistd.h> //for STDIN_FILENO
#include <sys/stat.h>
#include <fcntl.h>

#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>


=======
#include <signal.h>
>>>>>>> a14ce106792895a204a37d8707297a49e0fd47c7

#include "parser.h"
#include "print.h"
#include "forback.h"

/* --- symbolic constants --- */
#define HOSTNAMEMAX 100

/* --- use the /proc filesystem to obtain the hostname --- */
<<<<<<< HEAD
char *gethostnameFile(void)
{
  FILE *infile;
  char* c = malloc(1000);
  infile = fopen("/proc/sys/kernel/hostname", "r");
  fscanf(infile,"%s",c);
  fclose(infile);
  return c;
=======
char *gethostname(char* hostname)
{
  FILE *versionfile;

  char line[HOSTNAMEMAX];
  versionfile = fopen("/proc/sys/kernel/hostname","r");

  fgets(line,HOSTNAMEMAX,versionfile);
  sscanf(line,"%s",hostname); //if unable to scan, then hostname is already set, so no if(scan(..)) necessary

  fclose(versionfile);
  return hostname; //very unnecessary
>>>>>>> a14ce106792895a204a37d8707297a49e0fd47c7
}

/* --- execute a shell command --- */
int executeshellcmd (Shellcmd *shellcmd)
{
  struct _cmd *the_cmds = shellcmd->the_cmds;
  char* in      = shellcmd->rd_stdin;
  char* out     = shellcmd->rd_stdout;

  //Reversing the list for easier execution
  struct _cmd *next = NULL;
  struct _cmd *temp;
  int run = 1;

  while(run){
    if(the_cmds->next == NULL){
      run = 0;
    }
    else{
      temp = the_cmds->next;
    }
    
    the_cmds->next = next;
    
    next = the_cmds;
    if(run){  
      the_cmds = temp;
    }
  }
<<<<<<< HEAD
  //End of reversing list
=======
  // End of reversing list
  
  // Exit command, ctrl c command

  int fd[2];
>>>>>>> a14ce106792895a204a37d8707297a49e0fd47c7
  int inId, outId, closeId;
  outId = -1;
  inId  = open(in, O_RDONLY);
  int fid[2] = {inId, outId};
  while(the_cmds != NULL){

    char** cmd = the_cmds->cmd;
    if(the_cmds->next != NULL){
      if(pipe(fid) < 0){
        exit(1); //Not able to create pipe
      }
<<<<<<< HEAD
      outId = fid[1];
    }
    else{
      outId = open(out, O_WRONLY | O_CREAT, 0666);
    }
    
    closeId = fid[0];
=======

      closeId = fd[0];
      outId = fd[1];
    }else{
      closeId = -1;
      outId = -1;
    }
    
>>>>>>> a14ce106792895a204a37d8707297a49e0fd47c7

    if(shellcmd->background){
      backgroundcmd(*cmd, cmd, inId, outId, closeId); 
    }
    else{
      if(foregroundcmd(*cmd, cmd, inId, outId, closeId) == -1) 
      {
        exit(0); 
      }
    }

<<<<<<< HEAD
    if(fid[1] != -1){
      close(fid[1]);
    }
=======
    close(fd[1]);
    
>>>>>>> a14ce106792895a204a37d8707297a49e0fd47c7
    if(inId != -1){
      close(fid[0]);
    }

    the_cmds = the_cmds->next;
    inId = fid[0];  
  }

  return 0;
}

void interruptRun(int dummy){
  printf("%s", "caught ctrl-c");
  printf("%s\n", "if you want to exit the terminal, use Exit command or Ctrl+d");
}


/* --- main loop of the simple shell --- */
int main(int argc, char* argv[]) {

  /* initialize the shell */
  char *cmdline;
<<<<<<< HEAD
=======
  char hostname[HOSTNAMEMAX] = "Default";
>>>>>>> a14ce106792895a204a37d8707297a49e0fd47c7
  int terminate = 0;
  Shellcmd shellcmd;
  signal(SIGINT, interruptRun);

<<<<<<< HEAD
  char* hostname = gethostnameFile();
  if (hostname) {
=======
  if (gethostname(hostname)) {

>>>>>>> a14ce106792895a204a37d8707297a49e0fd47c7
    /* parse commands until exit or ctrl-c */
    while (!terminate) {

      printf("%s", hostname);
      if (cmdline = readline(":# ")) {
        if(*cmdline) {
          add_history(cmdline);
        
          if (parsecommand(cmdline, &shellcmd)) {
            terminate = executeshellcmd(&shellcmd);
          }
        }


        free(cmdline);
      } 
      else{  
        terminate = 1;
      }
    }
    free(hostname);
    printf("Exiting bosh.\n");
  }    
    
  return EXIT_SUCCESS;
}
