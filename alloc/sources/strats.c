#include "alloc/defines.h"
#include "alloc/tableio.h"
#include "alloc/types.h"
#include "core/defines.h"
#include <stddef.h>
alloc_function g_alloc_function;

// TODO(julianw): RETURNS address to new gap without allocating it
uint8_t *best_fit(size_t size, uint8_t *ignore) { return g_mem_start; }

uint8_t *worst_fit(size_t size, uint8_t *ignore) {

    size_t i = 0;
    size_t largest_gap = 0;
    uint8_t *lg_addr = NULL;
    while (g_mem_start + i < g_mem_end) {

        size_t gap_size = get_gap_size(g_mem_start + i, size, ignore);
        if (gap_size > largest_gap) {
            pr_info("Found gap at %p of size %zu", g_mem_start + i, size);
            largest_gap = gap_size;
            i += largest_gap;
            lg_addr = g_mem_start + i;
            continue;
        }
        i++;
    }
    pr_warning("Reached end of loop without a gap found. Storage is full");
    return nullptr;
}

uint8_t *first_fit(size_t size, uint8_t *ignore) {
    size_t i = 0;
    while (g_mem_start + i < g_mem_end) {

        size_t gap_size = get_gap_size(g_mem_start + i, size, ignore);
        if (gap_size >= size) {
            pr_info("Found gap at %p of size %zu", g_mem_start + i, size);
            return g_mem_start + i;
        }
        i++;
    }
    pr_warning("Reached end of loop without a gap found. Storage is full");
    return nullptr;
}

uint8_t *next_fit(size_t size, uint8_t *ignore) { return g_mem_start; }

void set_alloc_function(alloc_strat_e strat) {
    switch (strat) {
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
