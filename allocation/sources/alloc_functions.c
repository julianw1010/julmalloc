/**
 * @brief Implementation of allocation functions 
 */

#define _XOPEN_SOURCE 500
#include <stddef.h>
#include <unistd.h>
#include <stdio.h>

#include "core/defines.h"

#include "allocation/alloc_functions.h"

void *malloc_m(size_t size) {
    fprintf(stdout, "%d\n", sbrk(0));
    return NULL;
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
