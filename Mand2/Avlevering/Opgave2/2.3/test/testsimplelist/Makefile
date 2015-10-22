test: list.o concurrencyTest.o
	gcc -pthread list.o concurrencyTest.o -o test

list.o: list.c list.h
	gcc -c list.c

concurrencyTest.o: minunit.h concurrencyTest.c list.h
	gcc -c concurrencyTest.c

clean:
	rm *.o
