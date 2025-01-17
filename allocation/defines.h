/**
 * @brief Alloc defines
 */

#include <math.h>

#include "allocation/types.h"

extern MapAddr map_start;
extern MapAddr map_end;
extern MemAddr mem_start;
extern MemAddr mem_end;
extern alloc_function allocator;

#define HEAP_SIZE 100
#define TABLE_SIZE (int) floor(1/3*(100))
#define STORAGE_SIZE 2*TABLE_SIZE

#define FREE 0x0
#define ALLOCD 0x1
#define CONSEC 0xf

#define LOW_NIBBLE (1 << 7 | 1 << 6 | 1 << 5 | 1 << 4)
#define HIGH_NIBBLE (1 << 3 | 1 << 2 | 1 << 1 | 1 << 0)
#define HIGH_NIBBLE_OFFSET 4
#define LOW_NIBBLE_OFFSET 0