#include "alloc/tableio.h"
#include "alloc/defines.h"
#include "alloc/storage.h"
#include "alloc/types.h"
#include "core/defines.h"
#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static int set_map_value_helper(uint8_t *p_addr, uint8_t n, bool low);

static uint8_t *read_map_value_helper(const uint8_t *p_addr, bool low);

static uint8_t *get_map_addr(const uint8_t *m_addr);

static bool is_low(const uint8_t *m_addr);

int set_map_value(const uint8_t *m_addr, uint8_t v) {
    if (!is_mem_addr(m_addr)) {
        pr_error("Invalid address");
        return ERROR;
    }

    uint8_t *p_addr = get_map_addr(m_addr);
    bool low = is_low(m_addr);

    return set_map_value_helper(p_addr, v, low);
}

uint8_t *read_map_value(const uint8_t *m_addr) {
    if (!is_mem_addr(m_addr)) {
        pr_error("Invalid address %p, Addr-Beg: %p End %p", m_addr, g_mem_start,
                 g_mem_end);
        return nullptr;
    }

    uint8_t *p_addr = get_map_addr(m_addr);
    bool low = is_low(m_addr);

    return read_map_value_helper(p_addr, low);
}

int set_mem_value(uint8_t *m_addr, uint8_t v) {
    if (!is_mem_addr(m_addr)) {
        pr_error("Invalid address");
        return ERROR;
    }

    pr_info("Byte at %p is being changed to %d", m_addr, v);
    set_byte(m_addr, v);
    return SUCCESS;
}

uint8_t *read_mem_value(uint8_t *m_addr) {
    if (!is_mem_addr(m_addr)) {
        pr_error("Invalid address");
        return ERROR;
    }

    return read_byte(m_addr);
}

size_t get_gap_size(const uint8_t *m_addr, size_t target) {
    if (!is_gap_beginning(m_addr)) {
        pr_error("Not a beginning of a gap");
        return 0;
    }

    for (size_t i = 0; i < target; i++) {
        if ((m_addr + i) >= g_mem_end) {
            return i;
        }
        uint8_t *value = read_map_value(m_addr + i);

        if (!value) {
            pr_error("Read error");
            return 0;
        }

        if (*value != UNALLLOCATED) {
            return i;
        }
    }

    return target;
}

size_t get_segment_size(const uint8_t *m_addr) {
    if (!is_segment_beginning(m_addr)) {
        pr_error("Invalid arguments");
        return 0;
    }
    int i = 1;
    while (m_addr + i < g_mem_end) {
        uint8_t *value = read_map_value(m_addr + i);
        if (!value) {
            pr_error("Read error");
            return 0;
        }
        if (*value == ALLOCATED_CONSECUTIVE) {
            i++;
        } else {
            break;
        }
    }
    return i;
}

bool is_segment_beginning(const uint8_t *m_addr) {

    uint8_t *value = read_map_value(m_addr);

    if (!value) {
        pr_error("Read error");
        return false;
    }

    return *value == ALLOCATED_START;
}

bool is_gap_beginning(const uint8_t *m_addr) {

    uint8_t *mapvalue = read_map_value(m_addr);
    if (!mapvalue) {
        pr_error("Read error");
        return false;
    }

    if (*mapvalue != UNALLLOCATED) {
        pr_warning("Map value not UNALLLOCATED");
        return false;
    }
    if (m_addr == g_mem_start) {
        pr_info("Addr is beginning of space");
        return true;
    }

    uint8_t *mv_prev = read_map_value(m_addr - 1);

    if (!mv_prev) {
        pr_error("Read error");
        return false;
    }

    if (*mv_prev == UNALLLOCATED) {
        pr_info("Not beginning of GAP");
        return false;
    }

    pr_info("Beginning of gap");
    return true;
}

bool is_map_addr(const uint8_t *p_addr) {
    return (bool)(p_addr >= g_map_start && p_addr < g_map_end);
}

bool is_mem_addr(const uint8_t *m_addr) {
    return (bool)((m_addr >= g_mem_start) && (m_addr < g_mem_end));
}

static int set_map_value_helper(uint8_t *p_addr, uint8_t n, bool low) {
    if (!is_map_addr(p_addr)) {
        pr_error("Invalid address %p, Addr-Beg: %p End %p", p_addr, g_map_start,
                 g_map_end);
        return ERROR;
    }
    uint8_t to_write = 0;
    if (low) {
        to_write = (n << LOW_NIBBLE_OFFSET) | (read_byte(p_addr) & HIGH_NIBBLE);
    } else {
        to_write = (n << HIGH_NIBBLE_OFFSET) | (read_byte(p_addr) & LOW_NIBBLE);
    }
    pr_info("%s nibble at %p is being changed to %x. \n Total byte is being "
            "overwritten with %d",
            low ? "Lower" : "Higher", p_addr, n, to_write);
    set_byte(p_addr, to_write);
    return SUCCESS;
}

static uint8_t *read_map_value_helper(const uint8_t *p_addr, bool low) {
    if (!is_map_addr(p_addr)) {
        pr_error("Invalid address %p, Addr-Beg: %p End %p", p_addr, g_map_start,
                 g_map_end);
        return nullptr;
    }
    uint8_t n = 0;
    if (low) {
        n = (LOW_NIBBLE & read_byte(p_addr)) >> LOW_NIBBLE_OFFSET;
    } else {
        n = (HIGH_NIBBLE & read_byte(p_addr)) >> HIGH_NIBBLE_OFFSET;
    }
    // pr_info("Reading %s nibble %x at %p", low ? "Low" : "High", n,
    // p_addr);
    return n;
}

static uint8_t *get_map_addr(const uint8_t *m_addr) {
    if (!is_mem_addr(m_addr)) {
        pr_error("Not auint8_t*");
        return nullptr;
    }
    return (g_map_start + ((uint8_t)floor((double)(m_addr - g_mem_start) / 2)));
}

static bool is_low(const uint8_t *m_addr) {
    return (bool)((m_addr - g_mem_start) % 2);
}