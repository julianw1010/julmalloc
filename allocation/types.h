#include <stdint.h>

typedef uint8_t* MapAddr;
typedef uint8_t* MemAddr;
typedef uint8_t* Addr;
typedef uint8_t nibble;
typedef uint8_t value;

typedef MemAddr (*alloc_function)(size_t);

typedef enum alloc_strat_e {
    FIRST_FIT,
    NEXT_FIT,
    BEST_FIT,
    WORST_FIT
} alloc_strat_e;