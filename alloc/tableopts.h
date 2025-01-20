#ifndef ALLOC_TABLEOPTS_H
#define ALLOC_TABLEOPTS_H

#include "alloc/types.h"
#include <stddef.h>

void init_table ();

uint8_t *add_map_entry (uint8_t *addr, size_t size);

int remove_map_entry (uint8_t *start);

uint8_t *memset_zero (uint8_t *start);

uint8_t *move_mem (uint8_t *old, uint8_t *new, size_t size);

#endif
