#ifndef ALLOCATION_ALLOC_FUNCTIONS_H
#define ALLOCATION_ALLOC_FUNCTIONS_H

#include <stddef.h>

void *malloc(size_t size);

void free(void *ptr);

void *calloc(size_t nmemb, size_t size);

void *realloc(void * ptr, size_t size);

#endif