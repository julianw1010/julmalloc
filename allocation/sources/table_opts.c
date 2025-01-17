#include "allocation/types.h"
#include "allocation/defines.h"
#include "table_opts.h"
#include "check_constraints.h"
#include "allocation/mem_driver.h"

MapAddr map_start;
MemAddr map_end;
MemAddr mem_start;
MemAddr mem_end;

void init_table() {
    map_start = sbrk(HEAP_SIZE);
    map_end = map_start + TABLE_SIZE;
    mem_start = map_start + TABLE_SIZE;
    mem_end = mem_start + STORAGE_SIZE;
    if(sbrk(HEAP_SIZE) == (void *) -1) {
        pr_error("sbrk error");
        exit(1);
    }

    for(MapAddr i = map_start; i < map_end; i++) {
        write_nibble_to_map(i, FREE, false);
        write_nibble_to_map(i, FREE, true);
    }
}