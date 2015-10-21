all: fifo

OBJS = concurrentList.o concurrentMain.o 
LIBS= -lpthread

fifo: concurrentMain.o ${OBJS}
	gcc -o $@  ${LIBS} ${OBJS}

clean:
	rm -rf *.o fifo
