all: bosh

OBJS = parser.o print.o test.o forback.o redirect.o
LIBS= -lreadline -ltermcap
CC = gcc

bosh: ${OBJS}
	${CC} -o $@ ${OBJS} ${LIBS}

clean:
	rm -rf *.o bosh
