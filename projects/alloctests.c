
//Testclient for (m/re/c)-alloc and free tests

#include <stdio.h>
#include <stdlib.h>

#include "core/defines.h"
#include "alloc/methods.h"
#include "alloc/tableopts.h"

int main(int argc, char *argv[]) {
    init_table();
    for (int i = 0; i<100; i++) {
        int *number = calloc_custom(1, 2);
        if(!number) {
            pr_error("Could not alloc");
            pr_info("Successfully allocated %d numbers", i);
            exit(EXIT_FAILURE);
        }

        pr_info("Integer number2: Address %p, value %d", (void *) number, *number);
    }

    return 1;

}