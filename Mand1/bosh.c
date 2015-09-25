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
#include <sys/types.h>

#include "parser.h"
#include "print.h"
#include "forback.h"

/* --- symbolic constants --- */
#define HOSTNAMEMAX 100

/* --- use the /proc filesystem to obtain the hostname --- */
char *gethostname2(char *hostname)
{
  //Skal nok skrives om.
  gethostname(hostname, HOSTNAMEMAX);
  return hostname;
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
  //End of reversing list
  
  int fd[2];
  int inId, outId, closeId;
  outId = -1;
  inId  = -1;

  while(the_cmds != NULL){

    char** cmd = the_cmds->cmd;
    
    if(the_cmds->next != NULL){
      if(pipe(fd) < 0){
        exit(1); //Not able to create pipe
      }
      outId = fd[1];
    }else{
      outId = -1;
    }
    
    closeId = fd[0];

    if(shellcmd->background){
      backgroundcmd(*cmd, cmd, inId, outId, closeId); 
    }else{
      foregroundcmd(*cmd, cmd, inId, outId, closeId); 
    }

    if(fd[1] != -1){
      close(fd[1]);
    }
    if(inId != -1){
      close(inId);
    }

    the_cmds = the_cmds->next;
    inId = fd[0];  
  }

  return 0;
}


/* --- main loop of the simple shell --- */
int main(int argc, char* argv[]) {

  /* initialize the shell */
  char *cmdline;
  char hostname[HOSTNAMEMAX] = "asdads";
  int terminate = 0;
  Shellcmd shellcmd;

  if (gethostname2(hostname)) {

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
    printf("Exiting bosh.\n");
  }    
    
  return EXIT_SUCCESS;
}

