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

int main(int argc, char const *argv[])
{
	char buf[1];

	while(read(0, buf, sizeof(buf))>0) {
	   // read() here read from stdin charachter by charchter
	   //the buf[0] contains the charchter got by read()
	   printf("%c\n", *buf);
	}
	return 0;
}