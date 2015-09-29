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

#include "parser.h"
#include "print.h"
#include "forback.h"

/* --- symbolic constants --- */
#define HOSTNAMEMAX 100

/* --- use the /proc filesystem to obtain the hostname --- */
char *gethostname(char* hostname)
{
  FILE *versionfile;

  char line[HOSTNAMEMAX];
  versionfile = fopen("/proc/sys/kernel/hostname","r");

  fgets(line,HOSTNAMEMAX,versionfile);
  sscanf(line,"%s",hostname); //if unable to scan, then hostname is already set, so no if(scan(..)) necessary

  fclose(versionfile);
  return hostname; //very unnecessary
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
  // End of reversing list
  
  // Exit command, ctrl c command

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

      closeId = fd[0];
      outId = fd[1];
    }else{
      closeId = -1;
      outId = -1;
    }
    

    if(shellcmd->background){
      backgroundcmd(*cmd, cmd, inId, outId, closeId); 
    }else{
      foregroundcmd(*cmd, cmd, inId, outId, closeId); 
    }

    close(fd[1]);
    
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
  char hostname[HOSTNAMEMAX] = "Default";
  int terminate = 0;
  Shellcmd shellcmd;

  if (gethostname(hostname)) {

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

