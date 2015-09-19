#include <unistd.h> //for STDIN_FILENO
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

#include "redirect.h"

int main(int argc, char const *argv[])
{
	/* manipulate the file descriptor of the child process */
	int fid = open("hej", O_RDONLY);
	/* replace stdin of the child process with fid */
	close(0); /* 0 = stdin */
	dup(fid);
	/* close fid */
	close(fid);

	char ch;
	while(read(STDIN_FILENO, &ch, 1) > 0)
	{
		printf("%c\n",ch);
	}
	return 0;
}