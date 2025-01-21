/**
 * @brief Implementation of allocation functions
 */

#include "alloc/defines.h"
#include "alloc/tableio.h"
#include "alloc/tableopts.h"
#include "alloc/types.h"
#include "core/defines.h"
#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static bool table_inited = false;

void *malloc(size_t size) {

    pr_info("Allocating with size %zu", size);
    if (!table_inited) {
        init_table();
        table_inited = true;
    }

    if (!size) {
        pr_warning("Size zero");
        return nullptr;
    }

    uint8_t *new_a = g_alloc_function(size, NULL);

    if (!new_a) {
        pr_error("g_alloc_function failed.");
        return nullptr;
    }

    pr_info("Found a gap at address %p\n", new_a);

    int status = add_map_entry(new_a, size);

    if (status == ERROR) {
        pr_error("add_map_entry");
        return nullptr;
    }

    pr_info("Added map entry for address %p of size %zu\n", new_a, size);

    return new_a;
}

void free(void *ptr) {
    if (!table_inited) {
        init_table();
        table_inited = true;
    }

    if (!ptr) {
        pr_warning("Null pointer %ptr", ptr);
        return;
    }
    pr_info("%p", (uint8_t *)ptr);
    int status = remove_map_entry((uint8_t *)ptr);
    if (status == ERROR) {
        pr_error("UNALLOCATED error. Aborting");
        exit(EXIT_FAILURE);
    }
}

void *calloc(size_t nmemb, size_t size) {

    if (!table_inited) {
        init_table();
        table_inited = true;
    }

    if (!nmemb || !size) {
        pr_warning("Product of input is zero. No alloc");
        return nullptr;
    }

    uint8_t *new_a = (uint8_t *)malloc(nmemb * size);

    if (!new_a) {
        pr_error("Malloc error %s", strerror(errno));
        return nullptr;
    }

    pr_info("Valid pointer");

    int status = memset_zero(new_a);

    pr_info("Set memory to zero");
    if (status == ERROR) {
        pr_error("memset_zero");
        return NULL;
    }
    return new_a;
}

// Idea: Try to expand or shrink existing area, if possible.
// Otherwise:
// Remove old map entries, without modifying the bytes in the memory
// section.
// Then search for a new gap with the memory search algorithm which now thinks
// the previous segment is unallocated. Since the search algorithms are read
// only, this doesn't cause any problems.
// If the memory algorithm finds a new gap (which might also be the same address
// as the old address), copy memory from old address to new address.
// It is not possible that while copying, data from the old segment is
// overwritten before it is copied because the search algorithms will always
// allocate at the beginning of a gap, never in the middle. If the new address
// is the same as the old address, no data is being copied regardless.
// After copying the bytes, add map entires of the new size to the new address.
// Return the new address.
void *realloc(void *ptr, size_t size) {

    if (!table_inited) {
        init_table();
        table_inited = true;
    }

    //  (C23: Undefined behaviour)
    if (size == 0) {
        return nullptr;
    }

    if (!ptr) {
        pr_warning("ptr iss NULL. realloc acts like malloc");
        void *new_a = malloc(size);
        return new_a;
    }

    size_t old_size = get_segment_size(ptr);

    if (old_size == 0) {
        pr_warning("Invalid pointer");
        return nullptr;
    }

    if (old_size == size) {
        pr_warning("Same size, do nothing");
        return ptr;
    }

    // Try to expand or shrink first

    if (old_size < size) {
        size_t i = 0;
        while (old_size + i < size) {
            int status = set_map_value(ptr + old_size + i, UNALLOCATED);
            if (status == ERROR) {
                pr_error("Could not shrink space");
                return nullptr;
            }
        }
    }

    if (old_size + get_gap_size((uint8_t *)ptr + 1, size, nullptr) <= size) {
        size_t i = 0;
        while (old_size + i < size) {
            int status =
                set_map_value(ptr + old_size + i, ALLOCATED_CONSECUTIVE);
            if (status == ERROR) {
                pr_error("Could not expand space");
                return nullptr;
            }
        }

        return ptr;
    }

    uint8_t *new_a = g_alloc_function(size, ptr);

    // Could not realloc. Memory unchanged
    if (!new_a) {
        pr_error("No gap found");
        return nullptr;
    }

    int status = remove_map_entry(ptr);
    if (status == ERROR) {
        pr_error("Could not remove map entry");
        return nullptr;
    }

    // Add map entries for the new moved space
    status = add_map_entry(new_a, size);
    if (status == ERROR) {
        pr_error("Could not add new map entries");
        return nullptr;
    }

    // Copy memory from old space to new space
    status = copy_mem(ptr, new_a, old_size);
    if (status == ERROR) {
        pr_error("Could not move memory");
        return nullptr;
    }

    return (void *)new_a;
}