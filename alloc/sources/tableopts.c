#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>

#include "alloc/defines.h"
#include "alloc/types.h"
#include "core/defines.h"

#include "alloc/strats.h"
#include "alloc/tableio.h"
#include "alloc/tableopts.h"

uint8_t *g_map_start;
uint8_t *g_map_end;
uint8_t *g_mem_start;
uint8_t *g_mem_end;

void
init_table ()
{
    pr_info ("Total size: %zu Table size: %zu Storage size %zu", HEAP_SIZE,
             TABLE_SIZE, STORAGE_SIZE);
    g_map_start = (uint8_t *)sbrk (HEAP_SIZE);
    g_map_end = g_map_start + TABLE_SIZE;
    pr_info ("g_map_start: %p g_map_end %p", g_map_start, g_map_end);
    g_mem_start = g_map_end;
    g_mem_end = g_mem_start + STORAGE_SIZE;
    pr_info ("g_mem_start: %p g_mem_end %p", g_mem_start, g_mem_end);
    if (g_map_start == (void *)-1)
        {
            pr_error ("sbrk error");
            exit (EXIT_FAILURE);
        }

    set_alloc_function (FIRST_FIT);
    pr_info ("Set alloc function");

    for (uint8_t *iterator = g_mem_start; iterator < g_mem_end; iterator++)
        {
            if (set_map_value (iterator, FREE) == ERROR)
                {
                    pr_error ("Could not set map value");
                }
        }
    pr_info ("Initialized table to FREE all entries \n");
}

int
add_map_entry (const uint8_t *addr, size_t size)
{
    pr_info ("Addr %p size %zu", addr, size);

    if (get_gap_size (addr, size) < size)
        {
            pr_error ("Gap too small");
            return ERROR;
        }

    //  0x1  0xf  0xf  0xf...
    // 7654 3210 7654 3210

    if (set_map_value (addr, ALLOCD))
        {
            pr_error ("Could not set beginning of map area");
        }

    for (size_t i = 1; i < size; i++)
        {
            if (set_map_value ((addr + i), CONSEC))
                {
                    pr_error ("Could not set map value");
                }
        }
    return SUCCESS;
}

int
memset_zero (uint8_t *start)
{
    if (!is_segment_beginning (start))
        {
            pr_error ("Invalid parameters. start doesn't point to the "
                      "beginning of allocated space");
            return ERROR;
        }

    if (set_mem_value (start, FREE))
        {
            pr_error ("Could not set mem value");
            return ERROR;
        }

    int i = 1;
    while (read_map_value (start + i) == CONSEC)
        {
            if (start + i >= g_mem_end)
                {
                    pr_error ("Memory access violation");
                    return ERROR;
                }
            if (set_mem_value (start + i, FREE))
                {
                    pr_error ("Could not set mem value");
                }
            i++;
        }

    pr_info ("Zeroed %d memory bytes", i);
    return SUCCESS;
}

int
move_mem (uint8_t *old, uint8_t *new, size_t segment_size)
{
    if (!old | !new)
        {
            pr_error ("Invalid pointers");
            return ERROR;
        }

    if (!is_mem_addr (old) || !is_mem_addr (new))
        {
            pr_error ("Adress out of bounds");
            return ERROR;
        }
    if (old == new)
        {
            pr_info ("Nothing to do");
            return SUCCESS;
        }
    size_t i = 0;
    while (i < segment_size)
        {
            if (old + i >= g_mem_end)
                {
                    pr_error ("Memory access violation");
                    return ERROR;
                }
            if (set_mem_value (new + i, read_map_value (old + i)) == ERROR)
                {
                    pr_error ("Write error ");
                    return ERROR;
                }
            i++;
        }

    return SUCCESS;
}

int
remove_map_entry (const uint8_t *start)
{
    if (read_map_value (start) != ALLOCD)
        {
            pr_error ("Not a beginning of a segment");
            return ERROR;
        }

    if (set_map_value (start, 0x0))
        {
            pr_error ("Could not set map value");
        }

    int i = 1;
    while (start + i < g_mem_end && read_map_value (start + i) == CONSEC)
        {
            if (set_map_value (start + i, 0x0))
                {
                    pr_error ("Could not set map value");
                }
            i++;
        }

    pr_info ("Cleared %d map entries", i);
    return SUCCESS;
}
