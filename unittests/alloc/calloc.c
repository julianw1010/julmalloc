#include "alloc/types.h"
#include "unittests/defines.h"
#include <alloc/defines.h>

#include <stdlib.h>

bool is_aligned(void *ptr) { return (uintptr_t)ptr % ALIGNMENT == 0; }

static bool is_empty(uint8_t *addr, size_t size) {
    pr_info("Checking is storage is empty");
    for (size_t i = 0; i < size; i++) {
        if (*(addr + i) != 0) {
            pr_error("Not zero!!! Urgently check your implementation");
            return false;
        }
        pr_info("Checked byte %zu", i);
    }
    return true;
}

int main() {

    uint8_t *array[STORAGE_SIZE_TESTING];

    // i denotes the storage size
    for (int i = 1; i <= STORAGE_SIZE_TESTING; i *= 2) {

        // j is a helper variable to allocate STORAGE_SIZE_TESTING / i many
        // storages
        for (int j = 0; j < STORAGE_SIZE_TESTING / i; j++) {

            pr_info("Allocating elements of size i");

            // malloc STORAGE_SIZE_TESTING/i many storages of size i,
            // initialized with zero

            array[j] = calloc(1, i);

            // Check if allocated storages are set to zero
            if (!is_empty(array[j], i)) {

                return EXIT_FAILURE;
            }

            // Check if storages are aligned
            if (!is_aligned(array[j])) {

                return EXIT_FAILURE;
            }
        }

        for (int j = 0; j < STORAGE_SIZE_TESTING / i; j++) {
            free(array[j]);
        }
    }

    return EXIT_SUCCESS;
}