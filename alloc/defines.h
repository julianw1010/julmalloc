/**
 * @brief Alloc defines
 */
#ifndef ALLOC_DEFINES_H
#define ALLOC_DEFINES_H

#include "alloc/types.h"
#include <math.h>
#include <stdint.h>

extern uint8_t *g_map_start; /**< Address of start of map area inclusive */
extern uint8_t *g_map_end;   /**< Address of end of map area exclusive*/
extern uint8_t *g_mem_start; /**< Address of start of memory area inclusive */
extern uint8_t *g_mem_end;   /**< Address of end of memory area exclusive */
extern alloc_function g_alloc_function; /**< Function pointer  */

#define HEAP_SIZE                                                              \
    (size_t)10 /**< Size of heap including map table and memory area */
#define TABLE_SIZE                                                             \
    (size_t)floor(                                                             \
        ((double)1 / 3) *                                                      \
        (HEAP_SIZE)) /**< Size of floor, one third of the total size */
#define STORAGE_SIZE                                                           \
    (2 *                                                                       \
     TABLE_SIZE) /**< Size of the memory area, two thirds of the total size */

enum {
    UNALLOCATED = 0x0, /**< Map entry indicating unallocated space */

    ALLOCATED_START =
        0x1, /**< Map entry indicating beginning of allocated space */
    ALLOCATED_CONSECUTIVE = 0xf /**< Map entry indicating allocated space bigger
                    than 1, following ALLOCATED_START always */
};

enum {
    ZERO /**< Memory entry indicating a value of 0 */
};

enum {
    HIGH_NIBBLE = (1 << 7 | 1 << 6 | 1 << 5 |
                   1 << 4), /**< Bit mask to read get high nibble */
    LOW_NIBBLE =
        (1 << 3 | 1 << 2 | 1 << 1 | 1 << 0), /**< Bit mask to read low nibble */
    HIGH_NIBBLE_OFFSET = 4, /**<Bit shift to properly read high nibble */
    LOW_NIBBLE_OFFSET = 0   /**<Bit shift to properly read low nibble */
};

#endif