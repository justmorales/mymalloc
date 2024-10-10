CC=gcc
CCFLAGS= -g -Wall -std=c99 -fsanitize=address

core_test: test_cases.c mymalloc.o
	$(CC) -o $@ $^ $(CCFLAGS)

memgrind: memgrind.c mymalloc.o
	$(CC) -o $@ $^ $(CCFLAGS)

clean:
	rm -f *.o memgrind core_test
	
