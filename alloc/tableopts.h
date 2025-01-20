#ifndef ALLOC_TABLEOPTS_H
#define ALLOC_TABLEOPTS_H

#include <stddef.h>
#include <stdint.h>

void init_table ();

int add_map_entry (const uint8_t *addr, size_t size);

int remove_map_entry (const uint8_t *start);

int memset_zero (uint8_t *start);

int move_mem (uint8_t *old, uint8_t *new, size_t size);

bool check_heap_integrity ();

size_t get_heap_used_space ();

#endif
