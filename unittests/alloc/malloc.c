#include "alloc/defines.h"
#include "alloc/methods.h"
#include "alloc/tableopts.h"
#include "core/defines.h"
#include "math.h"
#include <stdlib.h>

int main(int argc, char *argv[]) {

    if (malloc(0)) {
        pr_error("Not null pointer");
        return EXIT_FAILURE;
    }

    for (size_t j = 1; j <= STORAGE_SIZE; j++) {
        // Fill table
        for (size_t i = 0; i < (size_t)floor(((int)(STORAGE_SIZE / j))); i++) {
            if (!malloc(j)) {
                return EXIT_FAILURE;
            }

            if (!check_heap_integrity()) {
                pr_error("Memory table corrupted");
                return EXIT_FAILURE;
            }
        }

        // Check if heap is filled
        if (get_heap_used_space() !=
            (size_t)floor((int)(STORAGE_SIZE / j)) * j) {
            pr_error("Allocated memory size discrepancy");
            return EXIT_FAILURE;
        }

        // Try to allocate full table
        if (malloc(j)) {
            pr_error("Memory allocated despite table full");
            return EXIT_FAILURE;
        }
        erase_table();
    }

    return EXIT_SUCCESS;
}