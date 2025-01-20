#ifndef ALLOC_METHODS_H
#define ALLOC_METHODS_H

#include <stddef.h>

void *malloc(size_t size);

void free(void *ptr);

void *calloc(size_t n_memb, size_t size);

void *realloc(void *ptr, size_t size);

#endif
