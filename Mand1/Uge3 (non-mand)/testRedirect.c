
#include <stdio.h>

#include "redirect.h"

int main(int argc, char const *argv[])
{
	char *argv2[argc];
	char *readFilename = (char*) "hej"; 
	char *executeFilename = (char*) "./hello"; 

	int i = 1;
	while(i < argc){
		argv2[i-1] = (char*) argv[i];
		i++;
	}
	
	argv2[i-1] = NULL;

	redirect_stdoutcmd(executeFilename, argv2, readFilename);

	return 0;
}