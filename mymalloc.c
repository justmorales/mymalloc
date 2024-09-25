#include <stdio.h>
#include <stdlib.h>
#include <mymalloc.h>


// Initialize the "heap"
#define MEMLENGTH 4096
static union {
    char bytes[MEMLENGTH];
    double not_used;
} heap;

/**
 * @brief Allocate data from the heap
 * 
 * @param [in] size size of chunk requested to be allocated
 */

void *mymalloc(size_t size, char *file, int line){
}

/**
 * @brief Free up data from the heap
 * 
 * @param [in] ptr void pointer to the chunk requested to be deallocated
 */
void myfree(void *ptr, char *file, int line) {
}