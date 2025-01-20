#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "alloc/defines.h"
#include "alloc/types.h"
#include "core/defines.h"

#include "alloc/storage.h"
#include "alloc/tableio.h"

static bool is_low(const uint8_t *addr);

static uint8_t *get_map_addr(const uint8_t *addr);

static int set_value(uint8_t *addr, uint8_t n, bool low);

static uint8_t read_value(const uint8_t *addr, bool low);

int set_map_value(const uint8_t *addr, uint8_t v) {
    if (!is_mem_addr(addr)) {
        pr_error("Invalid address");
        return ERROR;
    }

    uint8_t *mapaddr = get_map_addr(addr);
    bool low = is_low(addr);

    return set_value(mapaddr, v, low);
}

uint8_t read_map_value(const uint8_t *addr) {
    if (!is_mem_addr(addr)) {
        pr_error("Invalid address %p, Addr-Beg: %p End %p", addr, g_mem_start,
                 g_mem_end);
        exit(EXIT_FAILURE);
    }

    uint8_t *mapaddr = get_map_addr(addr);
    bool low = is_low(addr);

    return read_value(mapaddr, low);
}

int set_mem_value(uint8_t *addr, uint8_t v) {
    if (!is_mem_addr(addr)) {
        pr_error("Invalid address");
        return ERROR;
    }

    pr_info("Byte at %p is being changed to %d", addr, v);
    set_byte(addr, v);
    return SUCCESS;
}

size_t get_gap_size(const uint8_t *addr, size_t target) {
    if (!is_gap_beginning(addr)) {
        pr_error("Not a beginning of a gap");
        return 0;
    }

    for (size_t i = 0; i < target; i++) {
        if ((addr + i) >= g_mem_end || read_map_value(addr + i) != FREE) {
            return i;
        }
    }
    return target;
}

static bool is_low(const uint8_t *addr) {
    return (bool)((addr - g_mem_start) % 2);
}

bool is_mem_addr(const uint8_t *addr) {
    return (bool)((addr >= g_mem_start) && (addr < g_mem_end));
}

bool is_map_addr(const uint8_t *addr) {
    return (bool)(addr >= g_map_start && addr < g_map_end);
}

static int set_value(uint8_t *addr, uint8_t n, bool low) {
    if (!is_map_addr(addr)) {
        pr_error("Invalid address %p, Addr-Beg: %p End %p", addr, g_map_start,
                 g_map_end);
        return ERROR;
    }
    uint8_t to_write = 0;
    if (low) {
        to_write = (n << LOW_NIBBLE_OFFSET) | (read_byte(addr) & HIGH_NIBBLE);
    } else {
        to_write = (n << HIGH_NIBBLE_OFFSET) | (read_byte(addr) & LOW_NIBBLE);
    }
    pr_info("%s nibble at %p is being changed to %x. \n Total byte is being "
            "overwritten with %d",
            low ? "Lower" : "Higher", addr, n, to_write);
    set_byte(addr, to_write);
    return SUCCESS;
}

static uint8_t read_value(const uint8_t *addr, bool low) {
    if (!is_map_addr(addr)) {
        pr_error("Invalid address %p, Addr-Beg: %p End %p", addr, g_map_start,
                 g_map_end);
        exit(EXIT_FAILURE);
    }
    uint8_t n = 0;
    if (low) {
        n = (LOW_NIBBLE & read_byte(addr)) >> LOW_NIBBLE_OFFSET;
    } else {
        n = (HIGH_NIBBLE & read_byte(addr)) >> HIGH_NIBBLE_OFFSET;
    }
    pr_info("Reading %s nibble %x at %p", low ? "Low" : "High", n, addr);
    return n;
}

static uint8_t *get_map_addr(const uint8_t *addr) {
    if (!is_mem_addr(addr)) {
        pr_error("Not auint8_t*");
        return nullptr;
    }
    return (g_map_start + ((uint8_t)floor((double)(addr - g_mem_start) / 2)));
}

bool is_gap_beginning(const uint8_t *addr) {
    if (read_map_value(addr) != FREE) {
        pr_warning("Map value not FREE");
        return false;
    }
    if (addr == g_mem_start) {
        pr_info("Addr is beginning of space");
        return true;
    }
    if (read_map_value(addr - 1) == FREE) {
        pr_info("Not beginning of GAP");
        return false;
    }

    pr_info("Beginning of gap");
    return true;
}

bool is_segment_beginning(const uint8_t *addr) {
    return read_map_value(addr) == ALLOCD;
}

size_t get_segment_size(const uint8_t *addr) {
    if (!is_segment_beginning(addr)) {
        pr_error("Invalid arguments");
        return 0;
    }
    int i = 1;
    while (addr <= g_mem_end && read_map_value(addr + i) == CONSEC) {
        i++;
    }
    return i;
}

/*staticuint8_t* get_map_addr*(uint8_t* addr, bool low) {
    if(!is_map_addr*(addr)) {
        pr_error("Not auint8_t*");
        return nullptr;
    }
    return (g_mem_start + (uint8_t)(addr-g_map_start)*2+low);
}*/
