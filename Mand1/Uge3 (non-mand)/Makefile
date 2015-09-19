main: forback.o testRedirect hello
	gcc forback.o -o main

testRedirect: redirect.o testRedirect.c
	gcc redirect.o testRedirect.c -o testRedirect

redirect.o: redirect.c redirect.h
	gcc -c redirect.c
	
forback.o: forback.c forback.h hello.c
	gcc -c forback.c
	gcc hello.c -o hello
