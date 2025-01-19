#include "alloc/types.h"

byte_value
read_byte_value (addr_t addr)
{
    return *addr;
}

void
set_byte_value (addr_t addr, byte_value v)
{
    *addr = v;
}
