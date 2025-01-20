#include "alloc/types.h"

uint8_t
read_byte (const uint8_t *addr)
{
    return *addr;
}

void
set_byte (uint8_t *addr, uint8_t v)
{
    *addr = v;
}
