#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"


// Initialize the "heap"
#define MEMLENGTH 4096
#define HEADERSIZE sizeof(metadata)
#define round8(n) ((n+7) & ~7)

typedef struct {
    unsigned int size;
    unsigned int allocated;
} metadata;

static union {
    char bytes[MEMLENGTH];
    double not_used;
} heap;

static int initialized;

static void leak_checker() {
    int count = 0, bytes = 0;
    metadata* node = (metadata*)heap.bytes;

    // Traverse through heap
    while (node->allocated) {
        count++;
        bytes += node->size;
        node = node + node->size;
    }

    printf("\nmymalloc: %d bytes leaked in %d objects\n", bytes, count);
}

static void initialize_heap() {
    // Create initial header and set size to MEMLENGTH
    metadata* head = (metadata*)heap.bytes;
    head->size = MEMLENGTH - HEADERSIZE;
    head->allocated = 0;
    initialized = 1;
    atexit(leak_checker);
}

/**
 * @brief Allocate data from the heap
 * 
 * @param [in] size size of chunk requested to be allocated
 */
void *mymalloc(size_t size, char *file, int line){
    // Check if number of bytes requested is valid
    if (size <= 0 || size >= MEMLENGTH - HEADERSIZE) {
        printf("mymalloc: Size of bytes requested is too small or too large (%s:%d)\n", file, line);
        return NULL;
    }

    if (!initialized) initialize_heap();

    size = round8(size);

    metadata* curr = (metadata*)heap.bytes;

    while (curr < (metadata*)(heap.bytes + MEMLENGTH)) {
        leak_checker();
        if (!curr->allocated) {
            printf("%p is not allocated\n", curr);
            // If chunk is big enough to split into 2
            if (curr->size - size >= HEADERSIZE * 2) {
                metadata* new = (metadata*)((char*)curr + HEADERSIZE + size);
                printf("\nWHAT: %d\n", curr->size);
                new->size = curr->size - size;
                new->allocated = 0;

                curr->size = size;
                printf("\tCurrent chunk located %p of size %d\n", curr, curr->size + 8);
                printf("\tNew chunk located %p of size %d\n", new, new->size + 8);
            }
            // Return pointer to payload by adding 1 * sizeof(metadata)
            curr->allocated = 1;
            return (void*)(curr+1);
        }
        printf("%p is allocated by a chunk of size %d\n", curr, curr->size + 8);
        curr = (metadata*)((char*)curr + curr->size + HEADERSIZE);
    }

    printf("mymalloc: Insufficient memory for requested bytes (%s:%d)\n", file, line);
    return NULL;
}

/**
 * @brief Free up data from the heap
 * 
 * @param [in] ptr void pointer to the chunk requested to be deallocated
 */
void myfree(void *ptr, char *file, int line) {
	//header size invluded in payload size
	char *pointer = (char *)ptr;

    printf("%p\n",pointer);
	//check if the pointer exists
	if(pointer == NULL){
		fprintf(stderr, "%s:%d:free: Can't free NULL\n", file, line);
		return;
	}


	//check if pointer is inside the memory

	if(pointer < (char*)heap.bytes || pointer >= (char*)(heap.bytes + MEMLENGTH)){
		fprintf(stderr, "%s:%d:free: Pointer out of memory[]\n", file, line);
		return;
	}

	//finding the block we are working with
	//tbh maybe i should make a function to do this so that it can return null if we dont find the value we are looking for
	// char *iterate = (metadata *)(heap.bytes);
	// struct metadata *meta = (struct metadata *)iterate;
	
	metadata *meta = (metadata *)heap.bytes;

	while(meta->allocated != 0){
		if(meta >= (metadata*)(heap.bytes + MEMLENGTH)){
			break;
		}
		//move to the next block
		meta += HEADERSIZE + meta->size;
		
	}

	//we have the block we are working with and checking if it is malloced or not
	if(meta->allocated == 0){
		fprintf(stderr, "%s:%d:free Pointer not malloc-ed\n", file, line);
	return;
	}
	
	meta->allocated = 0;
	

	//merge();

	//merge time

	
	// struct metadata *current = (struct metadata *)iterate;
	// struct metadata *previous = NULL;
    metadata *prev = NULL;
    metadata *curr = (metadata *)heap.bytes;


 	while (curr < (metadata*)(heap.bytes + MEMLENGTH)) {
        if (prev && prev->allocated == 0 && curr->allocated == 0) {
            prev->size += sizeof(metadata) + curr->size;
        } else {
            prev = curr;
        }
        curr += sizeof(metadata) + curr->size;
    }






}
