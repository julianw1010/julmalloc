#include "alloc/tableopts.h"
#include "alloc/defines.h"
#include "alloc/strats.h"
#include "alloc/tableio.h"
#include "alloc/types.h"
#include "core/defines.h"
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

uint8_t *g_map_start;
uint8_t *g_map_end;
uint8_t *g_mem_start;
uint8_t *g_mem_end;

void init_table() {
    pr_info("Total size: %zu Table size: %zu Storage size %zu", HEAP_SIZE,
            TABLE_SIZE, STORAGE_SIZE);
    g_map_start = (uint8_t *)sbrk(HEAP_SIZE);
    g_map_end = g_map_start + TABLE_SIZE;
    pr_info("g_map_start: %p g_map_end %p", g_map_start, g_map_end);
    g_mem_start = g_map_end;
    g_mem_end = g_mem_start + STORAGE_SIZE;
    pr_info("g_mem_start: %p g_mem_end %p", g_mem_start, g_mem_end);
    if (g_map_start == (void *)-1) {
        pr_error("sbrk error");
        exit(EXIT_FAILURE);
    }

    set_alloc_function(FIRST_FIT);
    pr_info("Set alloc function");

    for (uint8_t *iterator = g_mem_start; iterator < g_mem_end; iterator++) {
        if (set_map_value(iterator, UNALLLOCATED) == ERROR) {
            pr_error("Could not set map value");
        }
    }
    pr_info("Initialized table to UNALLLOCATED all entries \n");
}

void erase_table() {
    for (uint8_t *iterator = g_mem_start; iterator < g_mem_end; iterator++) {
        if (set_map_value(iterator, UNALLLOCATED) == ERROR) {
            pr_error("Could not set map value");
            return;
        }
    }
    pr_info("Initialized table to UNALLLOCATED all entries \n");
}

int add_map_entry(const uint8_t *m_addr, size_t size) {
    pr_info("Addr %p size %zu", m_addr, size);

    if (get_gap_size(m_addr, size) < size) {
        pr_error("Gap too small");
        return ERROR;
    }

    //  0x1  0xf  0xf  0xf...
    // 7654 3210 7654 3210

    if (set_map_value(m_addr, ALLOCATED_START)) {
        pr_error("Could not set beginning of map area");
    }

    for (size_t i = 1; i < size; i++) {
        if (set_map_value((m_addr + i), ALLOCATED_CONSECUTIVE)) {
            pr_error("Could not set map value");
        }
    }
    return SUCCESS;
}

int memset_zero(uint8_t *m_addr) {
    if (!is_segment_beginning(m_addr)) {
        pr_error("Invalid parameters. start doesn't point to the "
                 "beginning of allocated space");
        return ERROR;
    }

    if (set_mem_value(m_addr, UNALLLOCATED)) {
        pr_error("Could not set mem value");
        return ERROR;
    }

    int i = 1;
    while (read_map_value(m_addr + i) == ALLOCATED_CONSECUTIVE) {
        if (m_addr + i >= g_mem_end) {
            pr_error("Memory access violation");
            return ERROR;
        }
        if (set_mem_value(m_addr + i, UNALLLOCATED)) {
            pr_error("Could not set mem value");
        }
        i++;
    }

    pr_info("Zeroed %d memory bytes", i);
    return SUCCESS;
}

int copy_mem(uint8_t *m_addr_old, uint8_t *m_addr_new, size_t segment_size) {
    if (!m_addr_old | !m_addr_new) {
        pr_error("Invalid pointers");
        return ERROR;
    }

    if (!is_mem_addr(m_addr_old) || !is_mem_addr(m_addr_new)) {
        pr_error("Adress out of bounds");
        return ERROR;
    }
    if (m_addr_old == m_addr_new) {
        pr_info("Nothing to do");
        return SUCCESS;
    }
    size_t i = 0;
    while (i < segment_size) {
        if (m_addr_old + i >= g_mem_end) {
            pr_error("Memory access violation");
            return ERROR;
        }
        if (set_mem_value(m_addr_new + i, read_map_value(m_addr_old + i)) ==
            ERROR) {
            pr_error("Write error ");
            return ERROR;
        }
        i++;
    }

    return SUCCESS;
}

int remove_map_entry(const uint8_t *m_addr) {
    if (read_map_value(m_addr) != ALLOCATED_START) {
        pr_error("Not a beginning of a segment");
        return ERROR;
    }

    if (set_map_value(m_addr, UNALLLOCATED)) {
        pr_error("Could not set map value");
    }

    int i = 1;
    while (m_addr + i < g_mem_end &&
           read_map_value(m_addr + i) == ALLOCATED_CONSECUTIVE) {
        if (set_map_value(m_addr + i, UNALLLOCATED)) {
            pr_error("Could not set map value");
        }
        i++;
    }

    pr_info("Cleared %d map entries", i);
    return SUCCESS;
}

size_t get_heap_used_space() {
    size_t size = 0;
    int i = 0;
    while (g_mem_start + i < g_mem_end) {
        if (read_map_value(g_mem_start + i) != UNALLLOCATED) {
            size++;
        }
        i++;
    }
    return size;
}

bool check_heap_integrity() {
    int i = 0;
    bool insegment = false;
    while (g_mem_start + i < g_mem_end) {
        uint8_t value = read_map_value(g_mem_start + i);
        if (value == UNALLLOCATED) {
            insegment = false;
        }
        if (value == ALLOCATED_START) {
            insegment = true;
        }
        if (!insegment && value == ALLOCATED_CONSECUTIVE) {
            pr_error("Integrity violation");
            return false;
        }
        i++;
    }
    return true;
}