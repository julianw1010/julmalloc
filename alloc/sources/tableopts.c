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
    g_map_start = (map_addr) sbrk(HEAP_SIZE);
    g_map_end = g_map_start + TABLE_SIZE;
    g_mem_start = g_map_start + TABLE_SIZE;
    g_mem_end = g_mem_start + STORAGE_SIZE;
    if(g_map_start == (void *) -1) {
        pr_error("sbrk error");
        exit(EXIT_FAILURE);
    }

    set_alloc_function(FIRST_FIT);

    for(map_addr iterator = g_mem_start; iterator < g_mem_end; iterator++) {
        set_map_value(iterator, FREE);
    }
}

mem_addr add_map_entry(mem_addr addr, size_t size) {
    if(get_gap_size(addr)<size) {
        return NULL;
    }
    for (size_t i = 0; i<size; i++) {
        set_map_value(addr, ALLOCD);
    }
    return addr;
}

mem_addr memset_zero(mem_addr start) {

}

mem_addr move_mem(mem_addr old, mem_addr new) {

}

nibble_value remove_map_entry(mem_addr addr) {

}

