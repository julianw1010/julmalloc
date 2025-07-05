
#include <stdlib.h>

// This unit test will pass if EXIT_FAILURE is called.
int main(int argc __attribute__((unused)),
         char *argv[] __attribute__((unused))) {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfree-nonheap-object"
    if (realloc((void *)1, 5)) {
        return EXIT_SUCCESS;
    }
#pragma GCC diagnostic pop

    return EXIT_SUCCESS;
}