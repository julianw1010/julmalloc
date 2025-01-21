#include "alloc/defines.h"
#include "alloc/methods.h"
#include "alloc/tableopts.h"
#include "core/defines.h"
#include "math.h"
#include <stdlib.h>

int main(int argc, char *argv[]) {

    if (calloc(0, 0) || calloc(0, 1) || calloc(1, 0)) {
        pr_error("Not null pointer");
        return EXIT_FAILURE;
    }

    for (size_t j = 1; j <= STORAGE_SIZE; j++) {
        // Fill table
        for (size_t i = 0; i < (size_t)floor(((int)(STORAGE_SIZE / j))); i++) {
            uint8_t *addr = calloc(1, j);
            if (!addr) {
                return EXIT_FAILURE;
            }

            if (!check_heap_integrity()) {
                pr_error("Memory table corrupted");
                return EXIT_FAILURE;
            }

            pr_info("Memory table ok");

            if (!check_mem_zero(addr)) {
                pr_error("Allocated memory not zero");
                return EXIT_FAILURE;
            }

            pr_info("Memory zero");
        }

        // Check if heap is filled
        if (get_heap_used_space() !=
            (size_t)floor((int)(STORAGE_SIZE / j)) * j) {
            pr_error("Allocated memory size discrepancy");
            return EXIT_FAILURE;
        }

        // Try to allocate full table
        if (calloc(1, j)) {
            pr_error("Memory allocated despite table full");
            return EXIT_FAILURE;
        }
        erase_table();
    }

    return EXIT_SUCCESS;
}