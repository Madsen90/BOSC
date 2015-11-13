
virtmem: main.o page_table.o disk.o program.o page_table.h frameSelecter.o
	gcc main.o page_table.o disk.o program.o frameSelecter.o -o virtmem

main.o: main.c
	gcc -w -Wall -g -c main.c -o main.o

frameSelecter.o: frameSelecter.c
	gcc -w -Wall -g -c frameSelecter.c -o frameSelecter.o

page_table.o: page_table.c page_table.h
	gcc -w -Wall -g -c page_table.c -o page_table.o

disk.o: disk.c
	gcc -w -Wall -g -c disk.c -o disk.o

program.o: program.c
	gcc -w -Wall -g -c program.c -o program.o

clean:
	rm -f *.o virtmem
