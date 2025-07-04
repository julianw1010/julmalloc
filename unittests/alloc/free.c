#include "unittests/defines.h"
#include <alloc/defines.h>

#include <stdlib.h>

int main() {

    uint8_t *array[STORAGE_SIZE_TESTING];

    //Create storages of size i
    for (int i = 1; i <= STORAGE_SIZE_TESTING; i *= 2) {

        // Allocate STORAGE_SIZE_TESTING/i many storages
        for (int j = 0; j < STORAGE_SIZE_TESTING / i; i++) {
            array[i] = malloc(i);
        }

        // Free STORAGE_SIZE_TESTING/i many storages
        for (int j = 0; j < STORAGE_SIZE_TESTING / i; i++) {
            free(array[i]);
        }
    }

    return EXIT_SUCCESS;
}