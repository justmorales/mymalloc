Project: My Little Malloc(), CS214 FALL2024 jpd247 - Justin De Jesus jm2663 - Justin Morales

Plan, split up the work from Malloc and Free, so Justin had to do the malloc and Justin had to do the free (LOL we are both named Justin!)

To test code, we made test case file that would test by cases 
    Case 1 - checks to see if malloc correctly allocates space in the memory 
    Case 2 - tests for ptr overlap during mallocs to see if the ptr to headers are correctly being assigned 
    Case 3 - tests for distinct bytes and that no bytes are being overwritten 
    Case 4 - tests both free is functioning correctly and whether coalesces is being performed as memory is being freed in random order
    Case 5 - tests to see if it will allocate the last 8 byte (impossible), since 16 bytes are needed to allocate 8 for memory and 8 for header 
    Case 6 - Tests to see if 8 byte alignment is performed when requested size is not divisible by 8

Implementation of malloc(),free(): Design for mymalloc.c, we had an header that would hold the size of the block we allocated and if it is allocated or not. These were taken as unsigned ints and for allocated, a 1 showed that it was allocated and a 0 showed that it was not allocated. Since we were working with the heap, we thought of this similar to a linked list,where we could get the next malloced memory, or free memory from the previous pointers we have. So in order to get to the next block to malloc, we would find a suitable area for it to get malloced, then to get to that spot get the previous pointer and traverse to it by adding the size of the block and the HEADERSIZE to it to get to a place where we could malloc. Making Malloc, it is supposed to return a pointer to free space that the user can work with, and free will set the allocated space to 0 and combine adjacent chunks if they are freed also.

Actual Code explanation Before we can make malloc and free, we have to initialize the heap, so using the heap made in the header, we make a metadata (struct that we made that contains the size and if its malloced) of the heap, so we initialize it by giving it the maximum amount of space we are working with, (4096), minus 8, because we need space for the HEADERSIZE. The mymalloc function takes three parameters; size, file, and line. The size parameter is used to reserve the requested appropiate memory space for the client. The file and line parameter is used to report where errors occured in the client code. In malloc, it will initialize the heap if not initialized, said like above. Then it will take the size requested and round up to a multiple of 8. Next it iterates until it finds a suitable amount of space at the earliest point it can by using a while loop to iterate through the whole entire heap. It will check if the free chunk is larger than the requested size and give it two header sizes, then it will split the chunk. It updates the heap with its new size by subtracting the amount requested and its HEADERSIZE. Then it will return the requested chunk as a pointer and mark it as allocated. myfree function takes three parameters, ptr, file and line. The ptr parameter is used to find the header that it matches with the suitable chunk and proceeds to free it afterwards. The file and line parameter is used to report where errors occured in the client code. It will always check if the ptr is even an ptr in the address space of the memory region if yes proceeds to check for a match between ptr and headers if not returns an error that is not within memory. It uses a while loop to iterate through all the headers in the heap, and will check if the the header matches up with the ptr given and see if it allocated. Then at the very end of free, it will always check if the chunk in front(if it can) and behind it if they have been freed, if they have they will merge and make a big block that can be used again in malloc, if not nothing happens.

Performance Tests:
    Average time of malloc/free run 120 times (1 byte): 0.000003 seconds
    Average time of malloc/free with an array of 120 pointers (1 byte): 0.000019 seconds
    Average time of random malloc/free run 120 times (1 byte): 0.000008 seconds
    Average time of reverse free (10 bytes) and allocating entire heap: 0.000031 seconds
    Average time of random malloc/free with an array of 120 pointers (random bytes): 0.000010 seconds

HOW TO COMPILE
$ make
$ ./memgrind
$ ./memtest
$ ./test_cases
