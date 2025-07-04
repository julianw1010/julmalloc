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

    pr_info("Alloc");
    uint8_t *anchor = malloc(1);
    anchor += ALIGNMENT + sizeof(struct seg_tail_s);
    ASSERT(is_aligned(anchor));

    for (size_t aligned_storage_s = PAGE_SIZE; aligned_storage_s <= STORAGE_SIZE_TESTING ; aligned_storage_s+=PAGE_SIZE) {

    size_t num_gaps = 1;

    // Measure maximum number of gaps. The last gap has to be strictly larger
    // than the previous ones
    while (sum_aligned(num_gaps+1) + (num_gaps+1)* ALIGNMENT +
               (2 * (num_gaps+1)+1) *
                   (sizeof(struct seg_head_s) + sizeof(struct seg_tail_s)) <=
           aligned_storage_s) {
        num_gaps++;
    }

    num_gaps--;

    uint8_t *segments[num_gaps-1];

    uint8_t *barriers[num_gaps-1];

    // Add "barriers between gaps"
    for (size_t i = 0; i < num_gaps - 1; i++) {
        pr_info("True");

        pr_info("Alloc");
        segments[i] = malloc(i + 1);

        pr_info("Alloc");
        uint8_t *barrier = malloc(1);
        barriers[i] = barrier;
        ASSERT((uintptr_t)segments[i] % ALIGNMENT == 0)
        ASSERT((uintptr_t)barrier % ALIGNMENT == 0)
        ASSERT(is_aligned(segments[i]) && is_aligned(barrier));

        pr_info("" FMT_UINTPTR, barrier - segments[i]);
        ASSERT(barrier == segments[i] + round_up(i + 1, ALIGNMENT) +
                              sizeof(struct seg_head_s) +
                              sizeof(struct seg_tail_s))
        ASSERT(segments[i] ==
               (uint8_t *)anchor + (sum_aligned(i) + i * ALIGNMENT +
                                    (2 * i) * (sizeof(struct seg_head_s) +
                                               sizeof(struct seg_tail_s)) +
                                    sizeof(struct seg_head_s)));
        ASSERT(barrier ==
               (uint8_t *)anchor + (sum_aligned(i + 1) + i * ALIGNMENT +
                                    (2 * i + 1) * (sizeof(struct seg_head_s) +
                                                   sizeof(struct seg_tail_s)) +
                                    sizeof(struct seg_head_s)));
    }

    for (size_t i = 0; i < num_gaps-1; i++) {
        pr_info("Freeing address %p", segments[i]);
        free(segments[i]);
    }


    set_alloc_function(WORST_FIT);

    // Allocate num_gaps many elements of ascending size and check if they
    // land in the right grid
    for (size_t i = 0; i < num_gaps/2; i++) {

        pr_info("Alloc");
        uint8_t *addr = malloc(i + 1);
        pr_info("Num gaps: %zu", num_gaps);
        ASSERT(is_aligned(addr));
        pr_info("i %zu aligned_storage %zu", i, aligned_storage_s);

        ASSERT(addr == (uint8_t *)anchor + sizeof(struct seg_head_s) + 
                            sum_aligned(num_gaps-1) +
                             (num_gaps-1) * ALIGNMENT +
                             (2 * (num_gaps-1)) *
                                 (sizeof(struct seg_head_s) +
                                  sizeof(struct seg_tail_s)));

        free(addr);
    }

    for (size_t i = 0; i < num_gaps-1; i++) {
        free(barriers[i]);
    }


}

    return EXIT_SUCCESS;
}

int main() {
    set_alloc_function(WORST_FIT);
    return (grid_test());
}