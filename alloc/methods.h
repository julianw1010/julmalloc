#ifndef ALLOC_METHODS_H
#define ALLOC_METHODS_H

#include <stddef.h>

void *malloc_custom(size_t size);

void free_custom(void *addr);

void *calloc_custom(size_t n_memb, size_t size);

void *realloc_custom(void *old_a, size_t size);

#endif