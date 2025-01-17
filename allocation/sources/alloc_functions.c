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

void *malloc_m(size_t size) {

    if(!size) {
        return NULL;
    }

    return;
}

void free_m(void *ptr) {
    return;
}

void *calloc_m(size_t nmemb, size_t size) {
    if(!nmemb || !size) {
        return NULL;
    }
    return NULL;
}

void *realloc_m(void * ptr, size_t size) {
    if(!size) {
        free_m(ptr);
        return ptr;
    } 
    return NULL;
}    
