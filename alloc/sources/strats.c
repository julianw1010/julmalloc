#include <stddef.h>

#include "alloc/defines.h"
#include "alloc/types.h"
#include "core/defines.h"

#include "alloc/tableio.h"
alloc_function g_alloc_function;

//TODO RETURNS address to new gap without allocating it
mem_addr best_fit(size_t size) {
    return g_mem_start;
}

mem_addr worst_fit(size_t size) {
    return g_mem_start;
}

mem_addr first_fit(size_t size) {
    mem_addr iterator = g_mem_start;
    while(iterator<g_mem_end) {
        if(read_map_value(iterator) != FREE) {
            pr_warning("Map entry at %p is NOT FREE", iterator);
            iterator++;
            continue;
        }

        pr_info("Map entry at %p is FREE", iterator);
        size_t gap_size = is_valid_gap(iterator, size);
        if(gap_size>=size) {
            pr_info("Found gap at %p of size %zu", iterator, size);
            return iterator;
        }
        iterator += gap_size;
    }
    pr_warning("Reached end of loop without a gap found");
    return NULL;
}

mem_addr next_fit(size_t size) {
    return g_mem_start;
}

void set_alloc_function(alloc_strat_e strat) {
    switch(strat) {
        case FIRST_FIT:
            g_alloc_function = &first_fit;
            break;
        case NEXT_FIT:
            g_alloc_function = &next_fit;
            break;
        case BEST_FIT:
            g_alloc_function = &best_fit;
            break;
        case WORST_FIT:
            g_alloc_function = &worst_fit;
            break;
    }
}