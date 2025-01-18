#include <stddef.h>
#include <unistd.h>
#include <stdlib.h>

#include "alloc/types.h"
#include "alloc/defines.h"
#include "core/defines.h"

#include "alloc/tableopts.h"
#include "alloc/tableio.h"
#include "alloc/strats.h"

map_addr g_map_start;
map_addr g_map_end;
mem_addr g_mem_start;
mem_addr g_mem_end;

void init_table() {
    pr_info("Total size: %zu Table size: %zu Storage size %zu", HEAP_SIZE, TABLE_SIZE, STORAGE_SIZE);
    g_map_start = (map_addr) sbrk(HEAP_SIZE);
    g_map_end = g_map_start+TABLE_SIZE;
    pr_info("g_map_start: %p g_map_end %p", g_map_start, g_map_end);
    g_mem_start = g_map_start + TABLE_SIZE;
    g_mem_end = g_mem_start + STORAGE_SIZE;
    pr_info("g_mem_start: %p g_mem_end %p", g_mem_start, g_mem_end);
    if(g_map_start == (void *) -1) {
        pr_error("sbrk error");
        exit(EXIT_FAILURE);
    }

    set_alloc_function(FIRST_FIT);
    pr_info("Set alloc function");

    for(map_addr iterator = g_mem_start; iterator < g_mem_end; iterator++) {
        if(set_map_value(iterator, FREE) == ERROR) {
            pr_error("Could not set map value");
        }
    }
    pr_info("Initialized table to FREE all entries");
}

mem_addr add_map_entry(mem_addr addr, size_t size) {
    pr_info("Addr %p size %zu", addr, size);
    if(is_valid_gap(addr,size)<size) {
        pr_error("Gap too small");
        return NULL;
    }
    for (size_t i = 0; i<size; i++) {
        if(set_map_value(addr, ALLOCD) == ERROR) {
            pr_error("Could not set map value");
        }
    }
    return addr;
}

mem_addr memset_zero(mem_addr start) {
    return NULL;
}

mem_addr move_mem(mem_addr old, mem_addr new) {
    return NULL;
}

int remove_map_entry(mem_addr addr) {
    return ERROR; 
}

