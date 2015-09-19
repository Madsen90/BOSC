#include <stdio.h>
#include "pipe.h"

int main(int argc, char const *argv[])
{
	char *argv2[argc];
	char *executeFilename1 = (char*) "./writer"; 
	char *executeFilename2 = (char*) "./reader"; 

	int i = 1;
	while(i < argc){
		argv2[i-1] = (char*) argv[i];
		i++;
	}
	
	argv2[i-1] = NULL;

	pipecmd(executeFilename1, argv2, executeFilename2, argv2);

	return 0;
}