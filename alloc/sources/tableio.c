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

static uint8_t read_map_value_helper(const uint8_t *p_addr, bool low, bool *ok);

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

uint8_t read_map_value(const uint8_t *m_addr, bool *ok) {
    *ok = true;
    if (!is_mem_addr(m_addr)) {
        pr_error("Invalid address %p, Addr-Beg: %p End %p", m_addr, g_mem_start,
                 g_mem_end);
        *ok = false;
        return 0;
    }

    uint8_t *p_addr = get_map_addr(m_addr);
    bool low = is_low(m_addr);

    return read_map_value_helper(p_addr, low, ok);
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

uint8_t read_mem_value(uint8_t *m_addr, bool *ok) {
    *ok = true;
    if (!is_mem_addr(m_addr)) {
        pr_error("Invalid address");
        *ok = false;
        return 0;
    }

    return read_byte(m_addr);
}

size_t get_gap_size(const uint8_t *m_addr, size_t target, uint8_t *ignore) {

    if (!m_addr) {
        pr_error("Invalid address");
    }

    if (!is_mem_addr(m_addr)) {
        pr_error("Not a memory address");
        return 0;
    }

    size_t i = 0;
    while (i < target) {
        if ((m_addr + i) >= g_mem_end) {
            return i;
        }
        bool ok = false;
        uint8_t value = read_map_value(m_addr + i, &ok);

        if (!ok) {
            pr_error("Read error");
            return 0;
        }

        if (value != UNALLOCATED) {
            if (ignore && (m_addr + i == ignore)) {
                size_t ignoresize = get_segment_size(ignore);
                i += ignoresize;
                continue;
            }
            return i;
        }
        i++;
    }

    return target;
}

size_t get_segment_size(const uint8_t *m_addr) {
    bool ok = true;
    if (!is_segment_beginning(m_addr, &ok) || !ok) {
        pr_error("Invalid arguments");
        return 0;
    }
    int i = 1;
    while (m_addr + i < g_mem_end) {
        bool ok = false;
        uint8_t value = read_map_value(m_addr + i, &ok);
        if (!ok) {
            pr_error("Read error");
            return 0;
        }
        if (value == ALLOCATED_CONSECUTIVE) {
            i++;
        } else {
            break;
        }
    }
    return i;
}

bool is_segment_beginning(const uint8_t *m_addr, bool *ok) {
    *ok = true;
    uint8_t value = read_map_value(m_addr, ok);

    if (!*ok) {
        pr_error("Read error");
        return false;
    }

    if (value != ALLOCATED_START) {
        pr_warning("Not segment beginning");
        return false;
    }

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
    uint8_t oldbyte = read_byte(p_addr);
    if (low) {
        to_write = (n << LOW_NIBBLE_OFFSET) | (oldbyte & HIGH_NIBBLE);
    } else {
        to_write = (n << HIGH_NIBBLE_OFFSET) | (oldbyte & LOW_NIBBLE);
    }
    pr_info("%s nibble at %p is being changed to %x. \n Total byte is being "
            "overwritten with %d",
            low ? "Lower" : "Higher", p_addr, n, to_write);
    set_byte(p_addr, to_write);
    return SUCCESS;
}

static uint8_t read_map_value_helper(const uint8_t *p_addr, bool low,
                                     bool *ok) {
    *ok = true;
    if (!is_map_addr(p_addr)) {
        pr_error("Invalid address %p, Addr-Beg: %p End %p", p_addr, g_map_start,
                 g_map_end);
        *ok = false;
        return 0;
    }
    uint8_t n = 0;
    uint8_t value = read_byte(p_addr);
    if (low) {
        n = (LOW_NIBBLE & value) >> LOW_NIBBLE_OFFSET;
    } else {
        n = (HIGH_NIBBLE & value) >> HIGH_NIBBLE_OFFSET;
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

    if (!is_mem_addr(m_addr)) {
        pr_error("Not a mem addr");
        return false;
    }

    return (bool)((m_addr - g_mem_start) % 2);
}