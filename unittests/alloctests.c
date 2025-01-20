
// Testclient for (m/re/c)-alloc and free tests

#include <stdio.h>
#include <stdlib.h>

#include "alloc/defines.h"
#include "core/defines.h"

#include "alloc/tableopts.h"

int
main (int argc, char *argv[])
{
    int i = 1;
    int *number1 = calloc (1, sizeof (int));
    while (i < 100)
        {
            int *number2 = calloc (1, sizeof (int));
            if (!number2)
                {
                    pr_error ("Could not alloc");
                    pr_info ("Successfully allocated %d numbers", i);
                    // exit(EXIT_FAILURE);
                    break;
                }
            pr_info ("Integer number2: Address %p, value %d", (void *)number2,
                     *number2);
            i++;
        }

    pr_info ("First int addr: %p Mem start : %p", (void *)number1, g_mem_start);
    while (i > 0)
        {
            free (g_mem_start + ((i - 1) * sizeof (int)));
            pr_info ("Freed number");
            i--;
        }

    pr_info ("Freed all entries");

    return EXIT_SUCCESS;
}
