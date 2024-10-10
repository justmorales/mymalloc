CC=gcc
CCFLAGS= -g -Wall -std=c99 -fsanitize=address

all: clean memgrind memtest core_tests

core_tests: core_tests.c mymalloc.o
	$(CC) -o $@ $^ $(CFLAGS)

memtest: memtest.c mymalloc.o
	$(CC) -o $@ $^ $(CFLAGS)

memgrind: memgrind.c mymalloc.o
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o memgrind core_tests memtest