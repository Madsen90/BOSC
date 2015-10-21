test: concurrentList.o concurrencyTest.o
	gcc -pthread concurrentList.o concurrencyTest.o -o test

concurrentList.o: concurrentList.c concurrentList.h
	gcc -c concurrentList.c

concurrencyTest.o: minunit.h concurrencyTest.c concurrentList.h
	gcc -c concurrencyTest.c

clean:
	rm *.o
