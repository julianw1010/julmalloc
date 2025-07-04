#include "alloc/defines.h"
#include "alloc/memory_mgmt.h"
#include "alloc/strats.h"

#include "unittests/defines.h"
#include <stdlib.h>

bool is_aligned(void *ptr) { return (uintptr_t)ptr % ALIGNMENT == 0; }

size_t sum_aligned(size_t num_gaps) {
    size_t sum = 0;

    for (size_t i = 1; i <= num_gaps; i++) {
        sum += (size_t)ceil((double)i / ALIGNMENT) * ALIGNMENT;
        if (i % ALIGNMENT == 0) {
            ASSERT((size_t) ceil((double)i / ALIGNMENT) == (size_t) ceil((double)(i - 1) / ALIGNMENT));
        }
    }

    ASSERT(sum % ALIGNMENT == 0);

    return sum;
}

// Create a grid of gap sizes ascending by 1, separated by allocated memory of
// size 1
// Like this:
// 1 * 2 * 3 * ...
// where the number denotes the gap size and * denotes allocated storage of size
// 1
// If first_fit works correctly, the first fitting gap should be allocated
int inverse_grid_test() {

    uint8_t *anchor = malloc(1);
    anchor += 1*ALIGNMENT + sizeof(struct seg_tail_s);
    ASSERT(is_aligned(anchor));

    for (size_t j = PAGE_SIZE; j <= STORAGE_SIZE_TESTING ; j*=2) {

    size_t aligned_storage_s = (j - j%ALIGNMENT);

    set_alloc_function(FIRST_FIT);

    size_t num_gaps = 0;

    // Measure maximum number of gaps. The first gap has to be strictly larger
    // than the previous ones
    while (sum_aligned(num_gaps + 1) + num_gaps * ALIGNMENT +
               (2 * num_gaps + 1) *
                   (sizeof(struct seg_head_s) + sizeof(struct seg_tail_s)) <=
           aligned_storage_s) {
        num_gaps++;
    }

    if (num_gaps == 0) {
        continue;
    }

    pr_info("Numgaps %zu", num_gaps);

    uintptr_t barriers[num_gaps-1];

    // Add "barriers between gaps"
    for (size_t i = 0; i < num_gaps - 1; i++) {
        size_t helper_size = aligned_storage_s -
                             (sum_aligned(i + 1) + (i + 1) * ALIGNMENT +
                              (2 * (i + 1) + 1) * (sizeof(struct seg_head_s) +
                                                   sizeof(struct seg_tail_s)));
        uint8_t *helper = malloc(helper_size);
        uint8_t *barrier = malloc(1);
        barriers[i] = (uintptr_t) barrier;
        ASSERT(helper == anchor + sizeof(struct seg_head_s));
        pr_info("%zu", i);
        ASSERT(barrier == (uint8_t *)anchor + sizeof(struct seg_head_s)+round_up(helper_size, ALIGNMENT)+ sizeof(struct seg_tail_s) + sizeof(struct seg_head_s));

        ASSERT(is_aligned(helper) && is_aligned(barrier));
        free(helper);
    }

    set_alloc_function(BEST_FIT);

    // Allocate num_gaps many elements of ascending size and check if they land
    // in the right gap
    for (size_t i = 0; i < round_down(num_gaps - 1, ALIGNMENT); i++) {
        uint8_t *addr = malloc(i + 1);
        if (!addr) {
            pr_error("Malloc failure");
        }
        ASSERT(is_aligned(addr));
        pr_info("" FMT_UINTPTR, (uintptr_t)addr);
        pr_info("Assertion %zu: " FMT_UINTPTR, i + 1,
                (uintptr_t)anchor +
                    (aligned_storage_s -
                     (sum_aligned(round_up(i + 1, ALIGNMENT)) +
                      (round_up(i + 1, ALIGNMENT) - 1) * ALIGNMENT +
                      (2 * (round_up(i + 1, ALIGNMENT) - 1) + 1) *
                          (sizeof(struct seg_head_s) +
                           sizeof(struct seg_tail_s)))) +
                    sizeof(struct seg_head_s));
        ASSERT(addr == (uint8_t *)anchor +
                           (aligned_storage_s -
                            (sum_aligned(round_up(i + 1, ALIGNMENT)) +
                             (round_up(i + 1, ALIGNMENT) - 1) * ALIGNMENT +
                             (2 * (round_up(i + 1, ALIGNMENT) - 1) + 1) *
                                 (sizeof(struct seg_head_s) +
                                  sizeof(struct seg_tail_s)))) +
                           sizeof(struct seg_head_s));

        free(addr);
    }

    // Free "barriers between gaps"
    for (size_t i = 0; i < num_gaps - 1; i++) {
        free((uint8_t *)barriers[i]);
    }

}

    return EXIT_SUCCESS;
}

int main() { return (inverse_grid_test()); }