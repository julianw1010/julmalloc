#include "alloc/defines.h"
#include "alloc/methods.h"
#include "alloc/tableopts.h"
#include "core/defines.h"
#include "math.h"
#include <stdlib.h>

// Testing of edge cases
static int specialcases() {

    // Check if nullptr is returned when reallocating to size 0
    uint8_t *addr = malloc(1);
    uint8_t *old_addr = addr;
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

    // end of test case

    erase_table();

    // Check if realloc acts like malloc on nullptr as input
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
static int simpleshrink() {
    erase_table();

    // Shrink elements of size j..
    for (size_t j = 1; j <= STORAGE_SIZE; j++) {

        // in each sub-step by l
        for (size_t l = 1; l <= j - 1; l++) {

            // Fill table with n_maxentries of size j
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
            if (get_heap_used_space() != n_maxentries * j) {
                pr_error("Allocated memory size discrepancy");
                return EXIT_FAILURE;
            }

            // Shrink n_maxentries many segments numreducts many times b l like
            // Example_ n_maxentries
            // 1 * * 1 * * 1 * * 1 * * 1 * * 1 * * 1 * *
            // 1 *   1 *   1 *   1 *   1 *   1 *   1 *
            // 1     1     1     1     1     1     1
            size_t numreducts = (size_t)ceil(((int)(j / l))) - 1;
            for (size_t k = 1; k <= numreducts; k++) {

                for (size_t i = 0; i < n_maxentries; i++) {
                    if (!realloc(g_mem_start + (i * j), j - (l * k))) {
                        pr_error("Realloc failed");
                        return EXIT_FAILURE;
                    }

                    if (!check_heap_integrity()) {
                        pr_error("Memory table corrupted");
                        return EXIT_FAILURE;
                    }
                }

                size_t sizemes = get_heap_used_space();
                size_t newsize = (j - (l * k)) * n_maxentries;
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

// This function allocates 1<=i<=STORAGE_SIZE many elements of size 1 equally
// distributed.
// Then, they get expanded in steps of 1<=m<=max_size until it is no longer
// possible without moving around. Repeat for every m and every i.
static int simpleexpand() {
    erase_table();

    // Allocate i elements of size 1
    for (size_t i = 1; i < STORAGE_SIZE; i++) {

        // The maximum segment number which each segment can be expanded without
        // requiring moving around
        size_t max_size = (size_t)floor(((int)(STORAGE_SIZE / i)));

        for (size_t m = 1; m < max_size; m++) {

            pr_info("%zu elements", i);

            for (size_t j = 0; j < i; j++) {
                add_map_entry(g_mem_start + (max_size * j), 1);
                if (!check_heap_integrity()) {
                    pr_error("Memory table corrupted");
                    return EXIT_FAILURE;
                }
            }

            // Check if heap is filled
            if (get_heap_used_space() != i) {
                pr_error("Allocated memory size discrepancy");
                return EXIT_FAILURE;
            }

            size_t numsteps = (size_t)floor(((int)(max_size / m)));
            for (size_t j = 1; j <= numsteps; j++) {
                for (size_t k = 0; k < i; k++) {
                    if (!realloc(g_mem_start + (max_size * k), m * j)) {
                        pr_error("Realloc failed");
                        return EXIT_FAILURE;
                    }

                    if (!check_heap_integrity()) {
                        pr_error("Memory table corrupted");
                        return EXIT_FAILURE;
                    }
                }

                // Check heap size
                if (get_heap_used_space() != m * j * i) {
                    pr_error("Allocated memory size discrepancy");
                    return EXIT_FAILURE;
                }
            }
            erase_table();
        }
    }

    erase_table();
    return EXIT_SUCCESS;
}

int main(int argc, char *argv[]) { return specialcases() || simpleexpand(); }