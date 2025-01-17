#include <stdlib.h>

#include "alloc/types.h"
#include "alloc/defines.h"
#include "core/defines.h"

#include "alloc/storage.h"

static int is_mem_addr(mem_addr addr);

static int is_low(mem_addr addr);

static int set_nibble_value(map_addr addr, nibble_value n, bool low);

static nibble_value read_nibble_value(mem_addr addr, bool low);

static int is_map_addr(map_addr addr);

static mem_addr get_mem_addr(map_addr addr, bool low);

static map_addr get_map_addr(mem_addr addr);

static bool is_gap_beginning(mem_addr addr);

int set_map_value(mem_addr addr, nibble_value v) {
    if(!is_mem_addr(addr)) {
        return ERROR;
    }

    map_addr map_addr = get_map_addr(addr);
    bool low = is_low(addr);

    return set_nibble_value(map_addr, v, low);
}

nibble_value read_map_value(mem_addr addr) {
    if(!is_mem_addr(addr)) {
        return ERROR;
    }
    
    map_addr map_addr = get_map_addr(addr);
    bool low = is_low(addr);

    return read_nibble_value(map_addr, low);
}

int set_mem_value(mem_addr addr, nibble_value v) {
    if(!is_mem_addr(addr)) {
        return ERROR;
    }

    set_byte_value(addr, v);
    return SUCCESS;
}

size_t get_gap_size(mem_addr addr) {
    if(!is_gap_beginning(addr)) {
        return 0;
    } else {
        mem_addr iterator = addr;
        while(iterator < g_mem_end && read_map_value(iterator) == FREE) {
            iterator++;
        }
        return (size_t) (iterator - addr);
    }
}

static int is_low(mem_addr addr) {
    if(!is_mem_addr(addr)) {
        pr_error("Not a mem_addr");
        return ERROR;
    }
    return (addr-g_mem_start) % 2;
}

static int is_mem_addr(mem_addr addr) {
    return (addr>=g_mem_start && addr < g_mem_end);
}

static int is_map_addr(map_addr addr) {
    return (addr>=g_map_start && addr < g_map_end);
}

static int set_nibble_value(map_addr addr, nibble_value n, bool low) {
    if(!is_map_addr(addr)) {
        return ERROR;
    }
    uint8_t mask;
    if(low) {
        mask = ~LOW_NIBBLE | (n << LOW_NIBBLE_OFFSET);
    } else {
        mask = ~HIGH_NIBBLE | (n << HIGH_NIBBLE_OFFSET);
    }

    set_byte_value(addr, read_byte_value(addr) & mask);
    return SUCCESS;
}

static nibble_value read_nibble_value(mem_addr addr, bool low) {
    if(!is_mem_addr(addr)) {
        return ERROR;
    }
    nibble_value n;
    if(low) {
        n = (LOW_NIBBLE & read_byte_value(addr)) >> LOW_NIBBLE_OFFSET;
    } else {
        n = (HIGH_NIBBLE & read_byte_value(addr)) >> HIGH_NIBBLE_OFFSET;
    }
    return n;
}

static mem_addr get_mem_addr(map_addr addr, bool low) {
    if(!is_map_addr(addr)) {
        pr_error("Not a map_addr");
        return NULL;
    }
    return (g_mem_start + (uint8_t)(addr-g_map_start)*2+low);
}

static map_addr get_map_addr(mem_addr addr) {
    if(!is_mem_addr(addr)) {
        pr_error("Not a mem_addr");
        return NULL;
    }
    return (g_map_start + ((uint8_t) floor((double) (addr - g_mem_start)/2)));
}

static bool is_gap_beginning(mem_addr addr) {
    if(read_map_value(addr) != FREE) {
        return false;
    } else if(addr == g_mem_start) {
        return true;
    } else if(read_map_value(addr) == FREE) {
        return false;
    } else {
        return true;
    }
}
