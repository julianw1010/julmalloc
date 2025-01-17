#include "allocation/types.h"
#include "allocation/defines.h"
#include <stddef.h>

alloc_function allocator;

MemAddr best_fit(size_t size) {

}

MemAddr worst_fit(size_t size) {

}

MemAddr first_fit(size_t size) {

}

MemAddr next_fit(size_t size) {

}

void set_allocator(alloc_strat_e strat) {
    switch(strat) {
        case FIRST_FIT:
            allocator = &first_fit;
            break;
        case NEXT_FIT:
            allocator = &next_fit;
            break;
        case BEST_FIT:
            allocator = &best_fit;
            break;
        case WORST_FIT:
            allocator = &worst_fit;
            break;
    }
}