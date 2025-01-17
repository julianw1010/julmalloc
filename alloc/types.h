#ifndef ALLOC_TYPES_H
#define ALLOC_TYPES_H

#include <stdint.h>
#include <stddef.h>

//!Table-specific typedefs
typedef uint8_t* map_addr;
typedef uint8_t* mem_addr;
typedef uint8_t* addr_t;
typedef uint8_t nibble_value;
typedef uint8_t byte_value;

//!Function pointer to allocator function being used
typedef mem_addr (*alloc_function)(size_t);

//!Enum describing allocation strategies
typedef enum {
    FIRST_FIT,
    NEXT_FIT,
    BEST_FIT,
    WORST_FIT
} alloc_strat_e;

#endif