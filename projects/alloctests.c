
//Testclient for (m/re/c)-alloc and free tests

#include "stdio.h"

#include "core/defines.h"
#include "alloc/methods.h"
#include "alloc/tableopts.h"

int main(int argc, char *argv[]) {
    init_table();
    int *number = malloc_custom(2);
    *number = 5;
    return 1;

}