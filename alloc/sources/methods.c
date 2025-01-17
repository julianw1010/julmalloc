/**
 * @brief Implementation of allocation functions 
 */

#define _XOPEN_SOURCE 500
#include <stddef.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "core/defines.h"
#include "alloc/types.h"
#include "alloc/defines.h"

#include "alloc/tableopts.h"

void *malloc_custom(size_t size) {

    if(!size) {
        return NULL;
    }

    mem_addr new_a = g_alloc_function(size);
    if(!new_a) {
        return NULL;
    }

    return (void *) add_map_entry(new_a, size);
}

void free_custom(void *addr) {
    if(remove_map_entry((mem_addr) addr) == ERROR) {
        pr_error("FREE error. Aborting");
        exit(EXIT_FAILURE);
    }
}

void *calloc_custom(size_t n_memb, size_t size) {
    if(!n_memb || !size) {
        return NULL;
    }

    mem_addr new_a = malloc_custom(n_memb*size);

    if(!new_a) {
        return NULL;
    }

    return (void *) memset_zero(new_a);
}

void *realloc_custom(void *old_a, size_t size) {
    if(!size) {
        free_custom(old_a);
        return NULL;
    } 

    mem_addr new_a = g_alloc_function(size);

    return (void *) move_mem(old_a, new_a);    
}    
