#ifndef ALLOCATION_ALLOC_FUNCTIONS_H
#define ALLOCATION_ALLOC_FUNCTIONS_H

#include <stddef.h>

void *malloc_m(size_t size);

void free_m(void *ptr);

void *calloc_m(size_t nmemb, size_t size);

void *realloc_m(void *ptr, size_t size);

#endif