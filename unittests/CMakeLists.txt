add_executable(malloc alloc/malloc.c)
target_link_libraries(malloc alloc)
add_executable(free alloc/free.c)
target_link_libraries(free alloc)
add_executable(special_free alloc/special_free.c)
target_link_libraries(special_free alloc)
add_executable(calloc alloc/calloc.c)
target_link_libraries(calloc alloc)


add_test(NAME malloc COMMAND malloc)
add_test(NAME free COMMAND free)
add_test(NAME "special_free" COMMAND special_free)
add_test(NAME calloc COMMAND calloc)

set_property(TEST malloc free "special_free" calloc
   PROPERTY
   ENVIRONMENT LD_PRELOAD=${CMAKE_SOURCE_DIR}/build/alloc/liballoc.so)

set_property(TEST "special_free"
   PROPERTY
   WILL_FAIL TRUE)

