#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"

#define DEBUG 0

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
    int count = 0;
    int bytes = 0;
    metadata* node = (metadata*)heap.bytes;

    while (node < (metadata*)(heap.bytes + MEMLENGTH)) {
        if (DEBUG) printf("checking node %p for leak...\n", node);
        if (node->allocated) {
            count = count + 1;
            if (DEBUG) printf("...adding %d to bytes from...%p\n", HEADERSIZE+node->size, node);
            bytes += HEADERSIZE + node->size;
        }
        node = (metadata*)((char*)node + HEADERSIZE + node->size);
    }

    printf("mymalloc: %d bytes leaked in %d objects\n", bytes, count);
}

static void initialize_heap() {
    // Create initial header and set size to MEMLENGTH
    metadata* head = (metadata*)heap.bytes;
    head->size = MEMLENGTH - HEADERSIZE;
    head->allocated = 0;
    initialized = 1;
    atexit(leak_checker);
}

void merge() {
    metadata *curr = (metadata *)heap.bytes;

 	while (curr < (metadata*)(heap.bytes + MEMLENGTH)) {
        metadata *next = (metadata*)((char*)curr + curr->size + HEADERSIZE);
        if (!curr->allocated) {
            // Check if next header is not end of memory and is unallocated
            if (next != (metadata*)(heap.bytes + MEMLENGTH) && next->allocated == 0 && next->size != 0) {
                curr->size += next->size + HEADERSIZE;
                merge();
                if (DEBUG) leak_checker();
            } else {
                // Check if next header is still in memory
                if (next < (metadata*)(heap.bytes + MEMLENGTH)) {
                    curr = next;
                } else 
                    return;
            }
        } else
            curr = next;
        return;
    }
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
        if (DEBUG) printf("size: %d vs %d\n", curr->size + HEADERSIZE, size + HEADERSIZE);
        if (!curr->allocated && curr->size >= size) {
            if (DEBUG) printf("\n%p is not allocated\n", curr);
            // If chunk is big enough to split into 2
            if (curr->size > size) {
                if (DEBUG) printf("Current chunk size (presplit): %d\n", curr->size);
                metadata* new = (metadata*)((char*)curr + HEADERSIZE + size);
                new->size = curr->size - size - HEADERSIZE;
                new->allocated = 0;

                curr->size = size;
                if (DEBUG) printf("\tCurrent chunk located %p of size %d\n", curr, curr->size + 8);
                if (DEBUG) printf("\tNew chunk located %p of size %d\n", new, new->size + 8);
            }
            // Return pointer to payload by adding 1 * sizeof(metadata)
            curr->allocated = 1;
            return (void*)(curr+1);
        }
        if (DEBUG) printf("%p is allocated by a chunk of size %d\n", curr, curr->size + 8);
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
	ptr = (void*)((char*)ptr - HEADERSIZE);

	if(ptr == NULL){
		fprintf(stderr, "%s:%d:free: Can't free NULL\n", file, line);
		return;
	}

	//check if pointer is inside the memory
	if((metadata*)ptr < (metadata*)heap.bytes || (metadata*)ptr >= (metadata*)((char*)(heap.bytes + MEMLENGTH))){
		fprintf(stderr, "%s:%d:free: Pointer out of memory[]\n", file, line);
		return;
	}

	metadata *meta = (metadata *)heap.bytes;

	while(meta < (metadata*)(heap.bytes + MEMLENGTH)){
		if (meta == ptr && meta->allocated){
            // if (DEBUG) printf("meta %p == requested %p\n", meta, ptr);
			// break;
            if (DEBUG) printf("\tfreeing up %p\n", meta);
            meta->allocated = 0;
            merge();
            return;
		} else {
            if (meta == ptr)
                fprintf(stderr, "%s%d:free: Calling free twice on same pointer\n", file, line);
        }
		//move to the next block
		meta = (metadata*)((char*)meta + meta->size + HEADERSIZE);
	}
    // if (DEBUG) printf("\tfreeing up %p\n", meta);
	// meta->allocated = 0;

        // modularize this into merge()
    
    // metadata *prev = NULL;
    // metadata *curr = (metadata *)heap.bytes;

 	// while (curr < (metadata*)(heap.bytes + MEMLENGTH)) {
    //     metadata *next = (metadata*)((char*)curr + curr->size + HEADERSIZE);
    //     if (!curr->allocated) {
    //         // Check if next header is not end of memory and is unallocated
    //         printf("Current header size: %d\n", curr->size);
    //         printf("Next header size: %d\n", next->size);
    //         if (next != (metadata*)(heap.bytes + MEMLENGTH) && next->allocated == 0) {
    //             printf("COALESCE");
    //             curr->size += next->size + HEADERSIZE;
    //             leak_checker();
    //         } else {
    //             if (next < (metadata*)(heap.bytes + MEMLENGTH)) {
    //                 curr = next;
    //             } else 
    //                 return;
    //         }
    //     } else
    //         curr = next;
    //     return;
    // }
}