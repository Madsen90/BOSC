all: bosh

OBJS = parser.o bosh.o forback.o redirect.o
LIBS= -lreadline -ltermcap
CC = gcc

bosh: ${OBJS}
	${CC} -o bin/bosh ${OBJS} ${LIBS}

clean:
	rm -rf *.o bosh
