/**
 * @brief Alloc defines
 */
#ifndef ALLOC_DEFINES_H
#define ALLOC_DEFINES_H

#include <math.h>

#include "alloc/types.h"

extern map_addr g_map_start;
extern map_addr g_map_end;
extern mem_addr g_mem_start;
extern mem_addr g_mem_end;
extern alloc_function g_alloc_function;

#define HEAP_SIZE (size_t)1000
#define TABLE_SIZE (size_t)floor (((double)1 / 3) * (HEAP_SIZE))
#define STORAGE_SIZE (2 * TABLE_SIZE)

enum
{
    FREE = 0x0,
    ALLOCD = 0x1,
    CONSEC = 0xf
};

enum
{
    HIGH_NIBBLE = (1U << 7U | 1U << 6U | 1U << 5U | 1U << 4U),
    LOW_NIBBLE = (1U << 3U | 1U << 2U | 1U << 1U | 1U << 0U),
    HIGH_NIBBLE_OFFSET = 4,
    LOW_NIBBLE_OFFSET = 0
};

#endif