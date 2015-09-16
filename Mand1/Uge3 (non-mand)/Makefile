main: forback.o hello
	gcc forback.o -o main
	
forback.o: forback.c forback.h hello.c
	gcc -c forback.c
	gcc hello.c -o hello
