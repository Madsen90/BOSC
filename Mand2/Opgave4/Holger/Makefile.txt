all: bank

OBJS = banker.o matrix.o
LIBS = -lpthread

bank: ${OBJS}
	gcc -o $@ ${LIBS} ${OBJS}

clean:
	rm -rf *.o bank
