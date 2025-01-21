#include "alloc/defines.h"
#include "alloc/methods.h"
#include "alloc/tableopts.h"
#include "core/defines.h"
#include "math.h"
#include <stdlib.h>

// This unit test will pass if EXIT_FAILURE is called.
int main(int argc, char *argv[]) {
    init_table();
    realloc(g_mem_start, 5);

    return EXIT_SUCCESS;
}