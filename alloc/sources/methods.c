/**
 * @brief Implementation of allocation functions 
 */

#define _XOPEN_SOURCE 500
#include <stddef.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "core/defines.h"
#include "alloc/types.h"
#include "alloc/defines.h"

#include "alloc/tableopts.h"

void *malloc(size_t size) {

    if(!size) {
        pr_warning("Size zero");
        return NULL;
    }

    mem_addr new_a = g_alloc_function(size);
    if(!new_a) {
        pr_error("g_alloc_function failed.");
        return NULL;
    }

    pr_info("Found a gap at address %p\n", new_a);

    new_a = add_map_entry(new_a, size);

    if(!new_a) {
        pr_error("add_map_entry");
        return NULL;
    }

    pr_info("Added map entry for address %p of size %zu\n", new_a, size);

    return new_a;
}

void free(void *ptr) {
    if(!ptr) {
        return;
    }
    pr_info("%p", (uint8_t *) addr);
    if(remove_map_entry((mem_addr) addr)) {
        pr_error("FREE error. Aborting");
        exit(EXIT_FAILURE);
    }
}

void *calloc(size_t n_memb, size_t size) {
    if(!n_memb || !size) {
        pr_warning("Product of input is zero. No alloc");
        return NULL;
    }

    mem_addr new_a = (mem_addr) malloc_custom(n_memb*size);

    if(!new_a) {
        pr_error("Malloc error %s", strerror(errno));
        return NULL;
    }

    return (void *) memset_zero(new_a);
}

void *realloc(void *ptr, size_t new_size) {
    if(!new_size) {
        pr_warning("size zero. Realloc acts like free");
        free_custom(ptr);
        return NULL;
    } 

    mem_addr new_a = g_alloc_function(new_size);

    if(!new_a) {
        pr_error("Realloc failed");
        return NULL;
    }

    new_a = move_mem(ptr, new_a, new_site);
    if(!new_a) {
        pr_error("Could not move memory");
        return NULL;
    }    
    return (void *) new_a;
}    
