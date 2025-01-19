#Nothing to do yet
add_executable(alloctests alloctests.c)
target_link_libraries(alloctests alloc)

add_test(NAME alloctests COMMAND alloctests)
set_property(TEST alloctests
   PROPERTY
   ENVIRONMENT LD_PRELOAD=${CMAKE_CURRENT_BINARY_DIR}/lib/liballoc.so)