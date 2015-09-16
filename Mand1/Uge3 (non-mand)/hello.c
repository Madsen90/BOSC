#include <stdio.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
	printf("%s\n", "childargs");
	int i = 0;

	while(i < argc){
		printf("%s\n",argv[i++]);
	}
	sleep(2);
	printf("%s\n", "Barnet er færdigt");
	return 0;
}