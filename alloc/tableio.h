#ifndef ALLOC_TABLEIO_H
#define ALLOC_TABLEIO_H

#include "alloc/types.h"

int set_map_value (const uint8_t *addr, uint8_t v);

uint8_t read_map_value (const uint8_t *addr);

int set_mem_value (uint8_t *addr, uint8_t v);

size_t get_gap_size (const uint8_t *addr, size_t target);

bool is_gap_beginning (const uint8_t *addr);

bool is_segment_beginning (const uint8_t *addr);

size_t get_segment_size (const uint8_t *addr);

bool is_map_addr (const uint8_t *addr);

bool is_mem_addr (const uint8_t *addr);

#endif
