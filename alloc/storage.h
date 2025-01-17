#ifndef ALLOC_STORAGE_H
#define ALLOC_STORAGE_H

#include "alloc/types.h"

byte_value read_byte_value(addr_t addr);

void set_byte_value(addr_t addr, byte_value v);
#endif