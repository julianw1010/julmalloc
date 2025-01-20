#include "alloc/defines.h"
#include "alloc/methods.h"
#include "alloc/tableopts.h"
#include "core/defines.h"
#include <stdlib.h>

int main(int argc, char *argv[]) {

    for (size_t i = 0; i < STORAGE_SIZE; i++) {
        if (!malloc(1)) {
            return EXIT_FAILURE;
        }

        if (!check_heap_integrity()) {
            pr_error("Memory table corrupted");
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}