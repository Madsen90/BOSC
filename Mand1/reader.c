#include <stdio.h>
#include <unistd.h>


int main(int argc, char const *argv[])
{
	printf("Jeg læser falgende besked: \n");

	char ch;
	
	while(read(STDIN_FILENO, &ch, 1) > 0)
	{
		printf("%c\n",ch);
	}

	return (0);
}