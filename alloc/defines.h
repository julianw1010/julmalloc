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

#define HEAP_SIZE (size_t) 1000
#define TABLE_SIZE (size_t) floor(((double)1/3)*(HEAP_SIZE))
#define STORAGE_SIZE 2*TABLE_SIZE

#define FREE 0x0
#define ALLOCD 0x1
#define CONSEC 0xf

#define HIGH_NIBBLE (1 << 7 | 1 << 6 | 1 << 5 | 1 << 4)
#define LOW_NIBBLE (1 << 3 | 1 << 2 | 1 << 1 | 1 << 0)
#define HIGH_NIBBLE_OFFSET 4
#define LOW_NIBBLE_OFFSET 0

#endif