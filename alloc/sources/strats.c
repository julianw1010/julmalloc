#include <stddef.h>

#include "alloc/defines.h"
#include "alloc/types.h"

alloc_function g_alloc_function;

//TODO RETURNS address to new gap without allocating it
mem_addr best_fit(size_t size) {
    return g_mem_start;
}

mem_addr worst_fit(size_t size) {
    return g_mem_start;
}

mem_addr first_fit(size_t size) {
    return g_mem_start;
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