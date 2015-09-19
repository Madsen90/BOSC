main: forback.o testRedirect
	gcc forback.o -o main

testRedirect: pipe.o redirect.o testRedirect.c
	gcc reader.c -o reader
	gcc writer.c -o writer
	gcc pipe.o redirect.o testRedirect.c -o testRedirect

redirect.o: redirect.c redirect.h
	gcc -c redirect.c
	
pipe.o: pipe.c pipe.h
	gcc -c pipe.c
