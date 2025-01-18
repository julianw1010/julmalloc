
//Testclient for (m/re/c)-alloc and free tests

#include "stdio.h"

#include "core/defines.h"
#include "alloc/methods.h"
#include "alloc/tableopts.h"

int main(int argc, char *argv[]) {
    init_table();
    int *number = malloc_custom(2);
    *number = 5;

    pr_info("Integer number: Address %p, value %d", (void *) number, *number);
    int *number2 = malloc_custom(2);
    *number2 = 10;

    pr_info("Integer number2: Address %p, value %d", (void *) number2, *number2);

    return 1;

}