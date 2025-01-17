#ifndef ALLOC_TABLEIO_H
#define ALLOC_TABLEIO_H

#include "alloc/types.h"

int set_map_value(mem_addr addr, nibble_value v);

nibble_value read_map_value(mem_addr addr);

int set_mem_entry(mem_addr addr, byte_value v);

size_t get_gap_size(map_addr addr);

#endif