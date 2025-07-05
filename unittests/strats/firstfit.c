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

// Create a grid of gap sizes ascending by 1, separated by allocated memory of
// size 1
// Like this:
// 1 * 2 * 3 * ...
// where the number denotes the gap size and * denotes allocated storage of size
// 1
// If first_fit works correctly, the first fitting gap should be allocated
int grid_test() {

    uint8_t *anchor = malloc(1);
    anchor += 1 * ALIGNMENT + sizeof(struct seg_tail_s);
    ASSERT(is_aligned(anchor));

    for (size_t j = PAGE_SIZE; j <= STORAGE_SIZE_TESTING; j *= 2) {

        size_t num_gaps = 0;

        size_t aligned_storage_s = (j - j % ALIGNMENT) * PAGE_SIZE;

        // Measure maximum number of gaps. The last gap has to be strictly
        // larger than the previous ones
        while (sum_aligned(num_gaps + 1) + num_gaps * ALIGNMENT +
                   (2 * num_gaps + 1) * (sizeof(struct seg_head_s) +
                                         sizeof(struct seg_tail_s)) <=
               aligned_storage_s) {
            num_gaps++;
        }

        if (num_gaps == 0) {
            continue;
        }

        uint8_t *segments[num_gaps];

        uint8_t *barriers[num_gaps - 1];

        // Add "barriers between gaps"
        for (size_t i = 0; i < num_gaps - 1; i++) {
            segments[i] = malloc(i + 1);
            uint8_t *barrier = malloc(1);
            barriers[i] = barrier;
            ASSERT(is_aligned(segments[i]) && is_aligned(barrier));

            pr_info("" FMT_UINTPTR, barrier - segments[i]);
            pr_info("Gaps: %zu, i: %zu, j: %zu", num_gaps, i, j);
            ASSERT(segments[i] ==
                   (uint8_t *)anchor + (sum_aligned(i) + i * ALIGNMENT +
                                        (2 * i) * (sizeof(struct seg_head_s) +
                                                   sizeof(struct seg_tail_s)) +
                                        sizeof(struct seg_head_s)));
            ASSERT(barrier == (uint8_t *)anchor +
                                  (sum_aligned(i + 1) + i * ALIGNMENT +
                                   (2 * i + 1) * (sizeof(struct seg_head_s) +
                                                  sizeof(struct seg_tail_s)) +
                                   sizeof(struct seg_head_s)));
            ASSERT(barrier == segments[i] + round_up(i + 1, ALIGNMENT) +
                                  sizeof(struct seg_head_s) +
                                  sizeof(struct seg_tail_s));
        }

        segments[num_gaps - 1] = malloc(num_gaps);
        ASSERT(segments[num_gaps - 1] ==
               (uint8_t *)anchor +
                   (sum_aligned(num_gaps - 1) + (num_gaps - 1) * ALIGNMENT +
                    (2 * (num_gaps - 1)) * (sizeof(struct seg_head_s) +
                                            sizeof(struct seg_tail_s)) +
                    sizeof(struct seg_head_s)));
        ASSERT(is_aligned(segments[num_gaps - 1]));

        for (size_t i = 0; i < num_gaps; i++) {
            free(segments[i]);
        }

        uint8_t *addrs[num_gaps];

        // Allocate num_gaps many elements of ascending size and check if they
        // land in the right grid
        for (size_t i = 0; i < num_gaps; i++) {
            uint8_t *addr = malloc(i + 1);
            ASSERT(addr ==
                   (uint8_t *)anchor + (sum_aligned(i) + i * ALIGNMENT +
                                        (2 * i) * (sizeof(struct seg_head_s) +
                                                   sizeof(struct seg_tail_s)) +
                                        sizeof(struct seg_head_s)));
            ASSERT(is_aligned(addr));
            addrs[i] = addr;
            // free(addr);
        }

        for (size_t i = 0; i < num_gaps - 1; i++) {
            free(barriers[i]);
        }

        for (size_t i = 0; i < num_gaps; i++) {
            free(addrs[i]);
        }
    }

    return EXIT_SUCCESS;
}

int main() {
    set_alloc_function(FIRST_FIT);
    return (grid_test());
}