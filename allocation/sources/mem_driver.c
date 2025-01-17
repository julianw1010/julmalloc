#include "allocation/types.h"
#include "allocation/check_constraints.h"
#include "defines.h"
#include "allocation/defines.h"
#include "allocation/mem_driver.h"


static int write_nibble(MapAddr addr, nibble n, bool low);

int write_byte_to_mem(MemAddr addr, value v) {
    if(!is_mem_addr(addr)) {
        return ERROR;
    }

    *addr = v;
    return OK;
}

int write_nibble_to_map(MapAddr addr, nibble n, bool low) {
    if(!is_map_addr(addr)) {
        return ERROR;
    }

    return write_nibble((Addr) addr, n, low);
}

static int write_nibble(Addr addr, nibble n, bool low) {

    uint8_t mask;
    if(low) {
        mask = LOW_NIBBLE | (n << LOW_NIBBLE_OFFSET);
    } else {
        mask = HIGH_NIBBLE | (n << HIGH_NIBBLE_OFFSET);
    }

    *addr &= mask;
    return OK;
    
}

bool is_low(MemAddr addr) {
    if(!is_mem_addr(addr)) {
        return false;
    }
    return (addr-mem_start) % 2;
}

int write_nibble_to_map(MapAddr addr, nibble n, bool low) {
    if(!is_map_addr(addr)) {
        return ERROR;
    }

    return write_nibble((Addr) addr, n, low);
}