/**
 * @file
 * @brief Allocation related typedefs
 */
#ifndef ALLOC_TYPES_H
#define ALLOC_TYPES_H

#include <stddef.h>
#include <stdint.h>

//! Function pointer to allocator function being used
typedef uint8_t *(*alloc_function)(size_t, uint8_t *ignore);

//! Enum describing allocation strategies
typedef enum { FIRST_FIT, NEXT_FIT, BEST_FIT, WORST_FIT } alloc_strat_e;

#endif