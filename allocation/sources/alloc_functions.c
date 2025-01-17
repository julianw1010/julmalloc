/**
 * @brief Implementation of allocation functions 
 */

#define _XOPEN_SOURCE 500
#include <stddef.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "core/defines.h"
#include "allocation/alloc_functions.h"
#include "allocation/types.h"

MemAddr start;

void init();

void *malloc_m(size_t size) {
    init();
    return start;
}

void free_m(void *ptr) {
    return;
}

void *calloc_m(size_t nmemb, size_t size) {
    return NULL;
}

void *realloc_m(void * ptr, size_t size) {
    return NULL;
}

void init() {
    start = sbrk(100);
    if(sbrk(100) == (void *) -1) {
        pr_error("sbrk error");
        exit(1);
    }
}
