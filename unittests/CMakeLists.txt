add_test(NAME alloctests COMMAND alloctests)

set_property(TEST alloctests
   PROPERTY
   ENVIRONMENT LD_PRELOAD=${CMAKE_SOURCE_DIR}/build/alloc/liballoc.so)