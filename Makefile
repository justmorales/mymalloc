CC=gcc
CCFLAGS= -g -Wall -std=c99 -fsanitize=address

memgrind: memgrind.c mymalloc.o
	$(CC) -o $@ $^ $(CCFLAGS)

clean:
	rm -f *.o memgrind