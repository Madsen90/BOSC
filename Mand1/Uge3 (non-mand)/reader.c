#include <stdio.h>
#include <unistd.h>


int main(int argc, char const *argv[])
{
	printf("Jeg læser fælgende besked: \n");

	char ch;
	
	while(read(STDIN_FILENO, &ch, 1) > 0)
	{
		printf("%c\n",ch);
	}

	sleep(2);
	return 0;
}