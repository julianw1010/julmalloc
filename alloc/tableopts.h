#ifndef ALLOC_TABLEOPTS_H
#define ALLOC_TABLEOPTS_H

#include "alloc/types.h"

void init_table ();

mem_addr add_map_entry (mem_addr addr, size_t size);

int remove_map_entry (mem_addr start);

mem_addr memset_zero (mem_addr start);

mem_addr move_mem (mem_addr old, mem_addr new);

#endif
