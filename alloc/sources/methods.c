/**
 * @brief Implementation of allocation functions
 */

#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "alloc/defines.h"
#include "alloc/types.h"
#include "core/defines.h"

#include "alloc/tableio.h"
#include "alloc/tableopts.h"

static bool table_inited = false;

void *
malloc (size_t size)
{
    if (!table_inited)
        {
            init_table ();
            table_inited = true;
        }

    if (!size)
        {
            pr_warning ("Size zero");
            return nullptr;
        }

    uint8_t *new_a = g_alloc_function (size);
    if (!new_a)
        {
            pr_error ("g_alloc_function failed.");
            return nullptr;
        }

    pr_info ("Found a gap at address %p\n", new_a);

    new_a = add_map_entry (new_a, size);

    if (!new_a)
        {
            pr_error ("add_map_entry");
            return nullptr;
        }

    pr_info ("Added map entry for address %p of size %zu\n", new_a, size);

    return new_a;
}

void
free (void *ptr)
{
    if (!table_inited)
        {
            init_table ();
            table_inited = true;
        }

    if (!ptr)
        {
            return;
        }
    pr_info ("%p", (uint8_t *)ptr);
    if (remove_map_entry ((uint8_t *)ptr))
        {
            pr_error ("FREE error. Aborting");
            exit (EXIT_FAILURE);
        }
}

void *
calloc (size_t nmemb, size_t size)
{

    if (!table_inited)
        {
            init_table ();
            table_inited = true;
        }

    if (!nmemb || !size)
        {
            pr_warning ("Product of input is zero. No alloc");
            return nullptr;
        }

    uint8_t *new_a = (uint8_t *)malloc (nmemb * size);

    if (!new_a)
        {
            pr_error ("Malloc error %s", strerror (errno));
            return nullptr;
        }

    return (void *)memset_zero (new_a);
}

void *
realloc (void *ptr, size_t size)
{

    if (!table_inited)
        {
            init_table ();
            table_inited = true;
        }

    if (!size)
        {
            pr_warning ("size zero. Realloc acts like free");
            free (ptr);
            return nullptr;
        }
    size_t segment_size = get_segment_size (ptr);

    if (segment_size == 0)
        {
            pr_warning ("Invalid pointer");
            return NULL;
        }

    uint8_t *new_a = g_alloc_function (size);

    if (!new_a)
        {
            pr_error ("Realloc failed");
            return nullptr;
        }

    new_a = move_mem (ptr, new_a, segment_size);
    if (!new_a)
        {
            pr_error ("Could not move memory");
            return nullptr;
        }
    return (void *)new_a;
}
