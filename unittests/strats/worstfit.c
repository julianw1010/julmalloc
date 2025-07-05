#include "alloc/defines.h"
#include "alloc/memory_mgmt.h"

#include "unittests/defines.h"
#include <stdlib.h>

bool is_aligned(void *ptr) { return (uintptr_t)ptr % ALIGNMENT == 0; }

size_t sum_aligned(size_t num_gaps) {
    size_t sum = 0;

    for (size_t i = 1; i <= num_gaps; i++) {
        sum += (size_t)ceil((double)i / ALIGNMENT) * ALIGNMENT;
    }

    ASSERT(sum % ALIGNMENT == 0);

    return sum;
}

int two_test() {

    uint8_t *expand = malloc(STORAGE_SIZE_TESTING);
    uint8_t *end = malloc(1);
    free(expand);

    for (size_t i = 1;
         i < STORAGE_SIZE_TESTING / ((sizeof(struct seg_tail_s) + ALIGNMENT +
                                      sizeof(struct seg_head_s)));
         i++) {
        uint8_t *buffer = malloc(i);
        uint8_t *barrier = malloc(1);

        free(buffer);

        uint8_t *test = malloc(1);

        pr_info("%zu", test);
        pr_info("%zu", barrier + 1 * ALIGNMENT + sizeof(struct seg_tail_s) +
                           sizeof(struct seg_head_s));

        ASSERT(test == barrier + 1 * ALIGNMENT + sizeof(struct seg_tail_s) +
                           sizeof(struct seg_head_s));

        free(test);

        free(barrier);
    }

    return EXIT_SUCCESS;
}

int main() {
    set_alloc_function(WORST_FIT);
    return (two_test());
}