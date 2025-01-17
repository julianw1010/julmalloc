
//Testclient for (m/re/c)-alloc and free tests

#include "stdio.h"

#include "core/defines.h"
#include "allocation/alloc_functions.h"

int main(int argc, char *argv[]) {
    int *number = malloc_m(2);
    *number = 5;
    return 1;


}