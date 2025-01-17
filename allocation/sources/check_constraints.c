#include "allocation/types.h"
#include "allocation/defines.h"
#include "allocation/table_opts.h"

bool is_mem_addr(MemAddr addr) {
    return (addr>=mem_start && addr < mem_end);
}

bool is_map_addr(MapAddr addr) {
    return (addr>=map_start && addr < map_end);
}