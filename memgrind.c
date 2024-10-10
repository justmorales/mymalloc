#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include "mymalloc.h"

/**
 * @brief Repeats a task 50 times
 * 
 * @param [in] task pointer to functon to repeat
 */
static double repeat50(void (*task)()) {
    double time = 0;
    clock_t start, end;

    for(int i = 0; i < 50; i++) {
        start = clock();
        (*task)();
        end = clock();
        time += ((double)end - start) / (CLOCKS_PER_SEC);
    }
    return time/50;
}

/**
 * @brief Run malloc 120 times with a one byte request
 */
static void oneByte120(){
    char *p;
    for(int i = 1; i <= 120; i++){
        p =(char*)malloc(1);
        free(p);
    }
}

/**
 * @brief Run malloc 120 times with a one byte request with all pointers stored in an array
 */
static void oneByteArray120(){
    char* ptrArr[120];
    for(int i = 1; i <= 120; i++){
        ptrArr[i-1] = (char*)malloc(1);
    }
    for(int j = 1; j <= 120; j++){
        free(ptrArr[j-1]);
    }
}

/**
 * @brief Run malloc 120 times with a one random byte request with all pointers stored in an array
 */
static void randomMallocOneByte120(){
    srand(time(NULL));
    char* arr[120];

    int alloc = 0;
    int counter = 0;
    int allocs = 0;
    int zero = 0;

    while (allocs < 120){
        alloc = rand()%2;
        if (alloc == 1){
            arr[counter] = (char*)malloc(1);
            if (counter == 0){
                zero = 1;
            }
            allocs++;
            counter++;
        } else {
            if(counter == 1 && zero == 1) {
                free(arr[counter-1]);
                zero = 0;
                counter--;
            }
            if(counter > 0){
                free(arr[counter-1]);
                counter--;
            }
        }
    }
    for (int i = 0; i < counter; i++){
        if(i == 0 && zero == 1){
            free(arr[i]);
        }
        if (i > 0){
            free(arr[i]);
        }
    }
}

/**
 * @brief Run free 120 times in reverse (relative to data order)
 */
static void reverseFree20Byte120(){
    char* arr[120];
    for(int i = 0; i < 120; i++){
        arr[i] = (char*)malloc(10);
    }
    for(int j = 119; j >= 0; j--){
        free(arr[j]);
    }

    char* ptr = (char*)malloc(4080);
    free(ptr);
}

/**
 * @brief Run free 120 times in reverse (relative to data order)
 */
static void randomAlloc120(){
    srand(time(NULL));
    char* arr[120];

    int alloc = 0;
    int counter = 0;
    int allocs = 0;
    int zero = 0;
    int random = 0;

    while (allocs < 120){
        alloc = rand()%2;
        if (alloc == 1){
            random = (rand()%10)+1;
            arr[counter] = (char*)malloc(random);
            if (counter == 0){
                zero = 1;
            }
            allocs++;
            counter++;
        }
        else{
            if(counter == 1 && zero == 1){
                free(arr[counter-1]);
                zero = 0;
                counter--;
            }
            if(counter > 0){
                free(arr[counter-1]);
                counter--;
            }
        }
    }
    for (int i = 0; i < counter; i++){
        if(i == 0 && zero == 1){
            free(arr[i]);
        }
        if (i > 0){
            free(arr[i]);
        }
    }

}

/**
 * @brief Run all tests and record benchmarks in standard output
 */
int main (){
    printf("Average time of malloc/free run 120 times (1 byte): %lf seconds\n", repeat50(oneByte120));
    printf("Average time of malloc/free with an array of 120 pointers (1 byte): %lf seconds\n", repeat50(oneByteArray120));
    printf("Average time of random malloc/free run 120 times (1 byte): %lf seconds\n", repeat50(randomMallocOneByte120));
    printf("Average time of reverse free (10 bytes) and allocating entire heap: %lf seconds\n", repeat50(reverseFree20Byte120));
    printf("Average time of random malloc/free with an array of 120 pointers (random bytes): %lf seconds\n", repeat50(randomAlloc120));
}