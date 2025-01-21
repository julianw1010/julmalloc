#include "alloc/defines.h"
#include "alloc/methods.h"
#include "alloc/tableopts.h"
#include "core/defines.h"
#include "math.h"
#include <stdlib.h>

int main(int argc, char *argv[]) {

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

        // Free table
        for (size_t i = 0; i < (size_t)floor(((int)(STORAGE_SIZE / j))); i++) {
            free(g_mem_start + (i * j));

            if (!check_heap_integrity()) {
                pr_error("Memory table corrupted");
                return EXIT_FAILURE;
            }
        }
    }

    return EXIT_SUCCESS;
}