#include "alloc/types.h"
#include <alloc/defines.h>
#include <inttypes.h>

#include <stdlib.h>

int main() {
    uintptr_t offset;
    for (int i = 0; i < 1000; i++) {
        if ((offset = (uintptr_t)malloc(i) % ALIGNMENT)) {
            pr_error("Invalid alignment " FMT_UINTPTR " %zu", (uintptr_t)offset,
                     (size_t)ALIGNMENT);
            return EXIT_FAILURE;
        }
    }

    if ((sizeof(seg_head_s) % ALIGNMENT)) {
        pr_error("Invalid size, not aligned");
        return EXIT_FAILURE;
    }
    if ((sizeof(seg_tail_s) % ALIGNMENT)) {
        pr_error("Invalid size, not aligned");
        return EXIT_FAILURE;
    }
    if ((sizeof(seg_list_head_s) % ALIGNMENT)) {
        pr_error("Invalid size, not aligned");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}