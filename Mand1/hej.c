#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h> //for STDIN_FILENO
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>


void hej(int i){ printf("%s\n","Kill Sig"); }

int main(int argc, char const *argv[])
{
	int i;
	signal(SIGINT, hej);
	for(i = 0; i < 100; i++){
		printf("%s\n", "Hje");
		sleep(1);
	}

	return 0;
}