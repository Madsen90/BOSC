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

#include <sys/stat.h>
#include <fcntl.h>

#include <sys/wait.h>
#include <stdlib.h>
#include <signal.h>

#include "parser.h"
#include "print.h"
#include "forback.h"

/* --- symbolic constants --- */
#define HOSTNAMEMAX 100

/* --- use the /proc filesystem to obtain the hostname --- */
void gethostname(char* hostname)
{
  FILE *versionfile;

  char line[HOSTNAMEMAX];
  versionfile = fopen("/proc/sys/kernel/hostnamen","r");

  fgets(line,HOSTNAMEMAX,versionfile);
  //if unable to scan, then hostname is already set, so no if(scan(..)) necessary
  sscanf(line,"%s",hostname); 

  fclose(versionfile);
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

  if(strcmp("exit", (const char*) *shellcmd->the_cmds->cmd) == 0)
  {
    return 1;
  }

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

  int inId, outId, closeId;
  outId = -1;
  inId  = -1;

  if(in){
    inId = open(in, O_RDONLY);
  }

  int fid[2] = {inId, outId};
  while(the_cmds != NULL){

    char** cmd = the_cmds->cmd;
    if(the_cmds->next != NULL){
      if(pipe(fid) < 0){
        exit(1); //Not able to create pipe
      }
      outId = fid[1];
    }
    else{
      if(out){
        outId = open(out, O_WRONLY | O_CREAT, 0666);
      }else{
        outId = -1;
      }
    }
    
    closeId = fid[0];

    if(shellcmd->background || the_cmds->next != NULL){
      backgroundcmd(*cmd, cmd, inId, outId, closeId); 
    }
    else{
      foregroundcmd(*cmd, cmd, inId, outId, closeId);  
    }
    if(fid[1] != -1){
      close(fid[1]);
    }
    if(inId != -1){
      close(inId);
    }

    the_cmds = the_cmds->next;
    inId = fid[0];  
  }

  if(outId == -1){
    close(outId);
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
  char hostname[HOSTNAMEMAX] = "Default";
  int terminate = 0;
  Shellcmd shellcmd;
  signal(SIGINT, interruptRun);
  

  gethostname(hostname);
  /* parse commands until exit or ctrl-d */
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
    
  return EXIT_SUCCESS;
}
