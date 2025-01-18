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
    g_mem_start = g_map_end;
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
    pr_info("Initialized table to FREE all entries \n");
}

mem_addr add_map_entry(mem_addr addr, size_t size) {
    pr_info("Addr %p size %zu", addr, size);
    /*if(is_valid_gap(addr,size)<size) {
        pr_error("Gap too small");
        return NULL;
    }*/
 
    //  0x1  0xf  0xf  0xf...
    // 7654 3210 7654 3210

    if(set_map_value(addr, ALLOCD)) {
        pr_error("Could not set beginning of map area");
    }

    for (size_t i = 1; i<size; i++) {
        if(set_map_value((addr+i), CONSEC)) {
            pr_error("Could not set map value");
        }
    }
    return addr;
}

mem_addr memset_zero(mem_addr start) {
    if(read_map_value(start)!= ALLOCD) {
        pr_error("Not a beginning of a segment");
        return NULL;
    }

    if(set_mem_value(start, 0x0)) {
        pr_error("Could not set mem value");
    }

    int i = 1;
    while(start+i<g_mem_end && read_map_value(start+i)==CONSEC) {
        if(set_mem_value(start+i, 0x0)) {
            pr_error("Could not set mem value");
        }
        i++;
    }

    pr_info("Zeroed %d memory bytes", i);
    return start;
}

mem_addr move_mem(mem_addr old, mem_addr new) {
    return NULL;
}

int remove_map_entry(mem_addr addr) {
    return ERROR; 
}

