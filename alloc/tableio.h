#ifndef ALLOC_TABLEIO_H
#define ALLOC_TABLEIO_H

#include "alloc/types.h"

int set_map_value (mem_addr addr, nibble_value v);

nibble_value read_map_value (mem_addr addr);

int set_mem_value (mem_addr addr, byte_value v);

bool is_valid_gap (map_addr addr, size_t target);

#endif
