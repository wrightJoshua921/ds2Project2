CFLAGS = -g -Wall
CC = gcc -lrt -lm

objects = qSim.o pQueue.o queue.o customer.o calc.o

test: $(objects)
	$(CC) -o qSim $(objects)

qSim.o: qSim.c queue.h calc.h pQueue.h
pQueue.o: pQueue.c pQueue.h queue.h
queue.o: queue.c queue.h
customer.o: customer.c customer.h
calc.o: calc.c calc.h

.PHONY : clean
clean:
	rm qSim $(objects)
