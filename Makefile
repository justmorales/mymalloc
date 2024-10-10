CC=gcc
CCFLAGS= -g -Wall -std=c99 -fsanitize=address

all: clean memgrind core_tests

core_tests: core_tests.c mymalloc.o
	$(CC) -o $@ $^ $(CFLAGS)

memgrind: memgrind.c mymalloc.o
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o memgrind core_tests