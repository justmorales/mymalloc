#include <stdio.h>
#include <stdlib.h>
#include <mymalloc.h>
typedef struct {
    unsigned int size;
    unsigned int allocated;
} metadata;

// Initialize the "heap"
#define MEMLENGTH 4096
static union {
    char bytes[MEMLENGTH];
    double not_used;
} heap;


//thinking about doing a function instead

// static void merge(){
// 	//merge time
// 	struct metadata *meta = (struct metadata *)iterate;
// 	struct metadata *previous = NULL;

// 	//iterate until the first block
// 	while(meta->allocated != 0){
// 		if((previous != NULL) && (previous->size == 0) && (meta->size == 0)){
// 			previous->size = meta->size + sizeof(metadata);
// 			if(previous->size > MEMLENGTH){
// 				fprintf(stderr, "Error: merge block invalid\n");
// 			}
// 		}else{
// 			previous = meta;
// 		}
// 	}
// }

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
	//header size invluded in payload size
	char *pointer = (char *)ptr;
	//check if the pointer exists
	if(pointer == NULL){
		fprintf(stderr, "%s:%d:free: Can't free NULL\n", file, line);
		return;
	}
	//check if pointer is inside the memory
	//if(pointer < (sizeof(struct metadata) + MEMLENGTH + sizeof(struct metadata)){
	//	fprinf(stderr, "%s:%d:free: Pointer out of memory[]\n", file, line);
	//	return;
	//}

	if(pointer < heap.bytes || pointer >= heap.bytes + MEMLENGTH){
		fprintf(stderr, "%s:%d:free: Pointer out of memory[]\n", file, line);
		return;
	}

	//finding the block we are working with
	//tbh maybe i should make a function to do this so that it can return null if we dont find the value we are looking for
	// char *iterate = (metadata *)(heap.bytes);
	// struct metadata *meta = (struct metadata *)iterate;

	char *iterate = heap.bytes;
	metadata *meta = (metadata *)iterate;

	while(meta->allocated != 0){
		if(iterate >= heap.bytes + MEMLENGTH){
			break;
		}
		//move to the next block
		iterate += sizeof(metadata) + meta->size;
		meta = (metadata *)iterate;
		
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
	char *current = heap.bytes;
    metadata *prev = NULL;
    metadata *curr = (metadata *)current;

	//iterate until the first block
	// while(current->allocated != 0){
	// 	if((previous != NULL) && (previous->size == 0) && (current->size == 0)){
	// 		previous->size = current->size + sizeof(struct metadata);
	// 		if(previous->size > MEMLENGTH){
	// 			fprintf(stderr, "Error: merge block invalid\n");
	// 	}else{
	// 		previous = current;
	// 		meta += sizeof(metadata) + meta->size;
	// 	}
	// }
 	while (current < heap.bytes + MEMLENGTH) {
        if (prev && prev->allocated == 0 && curr->allocated == 0) {
            prev->size += sizeof(metadata) + curr->size;
        } else {
            prev = curr;
        }
        current += sizeof(metadata) + curr->size;
        curr = (metadata *)current;
    }






}
