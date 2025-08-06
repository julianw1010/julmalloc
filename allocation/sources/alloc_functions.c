//Here malloc, calloc etc. will be implemented
#include <stddef.h>

#include "allocation/alloc_functions.h"

void *malloc(size_t size) {
    return NULL;
}

void free(void *ptr) {
    return;
}

void *calloc(size_t nmemb, size_t size) {
    return calloc;
}

void *realloc(void * ptr, size_t size) {
    return NULL;
}