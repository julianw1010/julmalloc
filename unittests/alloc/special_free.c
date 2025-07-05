#include "alloc/defines.h"

#include <stdlib.h>

// This unit test will pass if EXIT_FAILURE is called.
int main(int argc __attribute__((unused)),
         char *argv[] __attribute__((unused))) {

    // Test freeing of nullptr. Should continue without modifying pointer.
    void *ptr = nullptr;

    free(ptr);

    if (ptr) {

        pr_error("Pointer has been modified");

        return EXIT_SUCCESS;
    }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfree-nonheap-object"

    free((void *)1);

#pragma GCC diagnostic pop

    return EXIT_SUCCESS;
}