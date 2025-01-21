#include "alloc/defines.h"
#include "alloc/methods.h"
#include "alloc/tableopts.h"
#include "core/defines.h"
#include "math.h"
#include <stdlib.h>

// Testing of edge cases
static int specialcases() {
    uint8_t *addr = malloc(1);
    if (realloc(addr, 0)) {
        pr_error("Not null pointer");
        return EXIT_FAILURE;
    }

    if (!check_heap_integrity()) {
        pr_error("Memory violation");
        return EXIT_FAILURE;
    }

    if (get_heap_used_space() == 0) {
        pr_error("realloc freed despite not being allowed to do so");
        return EXIT_FAILURE;
    }

    erase_table();

    if (!realloc(nullptr, 1)) {
        pr_error("Did not allocate");
        return EXIT_FAILURE;
    }

    if (!check_heap_integrity()) {
        pr_error("Memory violation");
        return EXIT_FAILURE;
    }

    if (get_heap_used_space() != 1) {
        pr_error("realloc did not allocate");
        return EXIT_FAILURE;
    }

    erase_table();

    return EXIT_SUCCESS;
}

// This function allocates floor(STORAGE_SIZE/j) elements of size 1<j<=STORAGE
// then shrinks them all by one with each loop till each element has size 1.
// Each loop the function checks if the total allocated size shrunk by 1, and if
// the heap is still consistent
static int testshrink() {
    erase_table();

    for (size_t j = 1; j <= STORAGE_SIZE; j++) {

        for (size_t l = 1; l <= j - 1; l++) {

            // Fill table

            size_t n_maxentries = (size_t)floor(((int)(STORAGE_SIZE / j)));
            for (size_t i = 0; i < n_maxentries; i++) {
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

            size_t numreducts = ceil((j / l)) - 1;
            for (size_t k = 1; k <= numreducts; k++) {

                for (size_t i = 0; i < n_maxentries; i++) {
                    if (!realloc(g_mem_start + i * j, j - l * k)) {
                        pr_error("Realloc failed");
                        return EXIT_FAILURE;
                    }

                    if (!check_heap_integrity()) {
                        pr_error("Memory table corrupted");
                        return EXIT_FAILURE;
                    }
                }

                size_t sizemes = get_heap_used_space();
                size_t newsize = (j - l * k) * n_maxentries;
                if (sizemes != newsize) {
                    pr_error(
                        "Memory size %zu Allocated memory size mismatch for "
                        "size %zu. Expected %zu but got %zu",
                        STORAGE_SIZE, j, newsize, sizemes);
                    return EXIT_FAILURE;
                }
            }

            erase_table();
        }
    }

    erase_table();

    return EXIT_SUCCESS;
}

int main(int argc, char *argv[]) { return specialcases() || testshrink(); }