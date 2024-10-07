CC=gcc
CCFLAGS= -g -Wall -std=c99 -fsanitize=address

memgrind: memgrind.c mymalloc.o
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o memgrind