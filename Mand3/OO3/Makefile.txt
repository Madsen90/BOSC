virtmem: main.o page_table.o disk.o program.o frameSelecter.o
	gcc main.o page_table.o disk.o program.o frameSelecter.o -o virtmem

main.o: main.c
	gcc -w -Wall -g -c main.c -o main.o

frameSelecter.o: frameSelecter.c frameSelecter.h
	gcc -w -Wall -g -c frameSelecter.c -o frameSelecter.o

disk.o: disk.c
	gcc -w -Wall -g -c disk.c -o disk.o

program.o: program.c
	gcc -w -Wall -g -c program.c -o program.o

test: page_table.o disk.o frameSelecter.o test.o
	gcc test.o page_table.o disk.o frameSelecter.o -o test

test.o: test.c
	gcc -w -Wall -g -c test.c -o test.o

clean:
	rm -f *.o virtmem
