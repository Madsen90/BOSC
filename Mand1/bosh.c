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

/* --- symbolic constants --- */
#define HOSTNAMEMAX 100

/* --- use the /proc filesystem to obtain the hostname --- */
char *gethostname2(char *hostname)
{
  gethostname(hostname, HOSTNAMEMAX);
 // char hostname2[HOSTNAMEMAX] = "asdasdasd";
 // hostname = &hostname2;
  return hostname;
}

/* --- execute a shell command --- */
int executeshellcmd (Shellcmd *shellcmd)
{
  printshellcmd(shellcmd);


  printf("%s\n", *(shellcmd->the_cmds->cmd));

  char* argv[2];

  argv[0] = "./writer";
  argv[0] = NULL;
  execvp( *(shellcmd->the_cmds->cmd), argv);
  
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

