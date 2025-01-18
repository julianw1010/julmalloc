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

//static mem_addr get_mem_addr(map_addr addr, bool low);

static map_addr get_map_addr(mem_addr addr);

static bool is_gap_beginning(mem_addr addr);

int set_map_value(mem_addr addr, nibble_value v) {
    if(!is_mem_addr(addr)) {
        pr_error("Invalid address");
        return ERROR;
    }

    map_addr map_addr = get_map_addr(addr);
    bool low = is_low(addr);

    return set_nibble_value(map_addr, v, low);
}

byte_value read_map_value(mem_addr addr) {
    if(!is_mem_addr(addr)) {
        pr_error("Invalid address %p, Addr-Beg: %p End %p", addr, g_mem_start, g_mem_end);
        return ERROR;
    }
    
    map_addr map_addr = get_map_addr(addr);
    bool low = is_low(addr);

    return read_nibble_value(map_addr, low);
}

int set_mem_value(mem_addr addr, byte_value v) {
    if(!is_mem_addr(addr)) {
        pr_error("Invalid address");
        return ERROR;
    }

    pr_info("Byte at %p is being changed to %d", addr, v);
    set_byte_value(addr, v);
    return SUCCESS;
}

size_t is_valid_gap(mem_addr addr, size_t target) {
    if(read_map_value(addr)!= FREE) {
        pr_error("Current map value not free");
        return 0;
    } else {

        for(size_t i = 0 ; i<target ; i++) {
            if((addr + i) >= g_mem_end || read_map_value(addr + i) != FREE) {
                return i;
            }
        }
        return target;
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
    return ((addr>=g_mem_start) && (addr < g_mem_end));
}

static int is_map_addr(map_addr addr) {
    return (addr>=g_map_start && addr < g_map_end);
}

static int set_nibble_value(map_addr addr, nibble_value n, bool low) {
    if(!is_map_addr(addr)) {
        pr_error("Invalid address %p, Addr-Beg: %p End %p", addr, g_map_start, g_map_end);
        return ERROR;
    }
    uint8_t to_write;
    if(low) {
        to_write = (n << LOW_NIBBLE_OFFSET) | (read_byte_value(addr) & HIGH_NIBBLE);
    } else {
        to_write = (n << HIGH_NIBBLE_OFFSET) | (read_byte_value(addr) & LOW_NIBBLE);
    }
    pr_info("%s nibble at %p is being changed to %x. \n Total byte is being overwritten with %d", low ? "Lower" : "Higher", addr, n, to_write);
    set_byte_value(addr, to_write);
    return SUCCESS;
}

static nibble_value read_nibble_value(map_addr addr, bool low) {
    if(!is_map_addr(addr)) {
        pr_error("Invalid address %p, Addr-Beg: %p End %p", addr, g_map_start, g_map_end);
        return ERROR;
    }
    nibble_value n;
    if(low) {
        n = (LOW_NIBBLE & read_byte_value(addr)) >> LOW_NIBBLE_OFFSET;
    } else {
        n = (HIGH_NIBBLE & read_byte_value(addr)) >> HIGH_NIBBLE_OFFSET;
    }
    pr_info("Reading %s nibble %x at %p", low ? "Low" : "High", n, addr);
    return n;
}

/*static mem_addr get_mem_addr(map_addr addr, bool low) {
    if(!is_map_addr(addr)) {
        pr_error("Not a map_addr");
        return NULL;
    }
    return (g_mem_start + (uint8_t)(addr-g_map_start)*2+low);
}*/

static map_addr get_map_addr(mem_addr addr) {
    if(!is_mem_addr(addr)) {
        pr_error("Not a mem_addr");
        return NULL;
    }
    return (g_map_start + ((uint8_t) floor((double) (addr - g_mem_start)/2)));
}

/*static bool is_gap_beginning(mem_addr addr) {
    if(read_map_value(addr) != FREE) {
        pr_warning("Map value not FREE");
        return false;
    } else if(addr == g_mem_start) {
        pr_info("Addr is beginning of space");
        return true;
    } else if(read_map_value(addr-1) == FREE) {
        pr_info("Not beginning of GAP");
        return false;
    } else {
        pr_info("Beginning of gap");
        return true;
    }
}*/
