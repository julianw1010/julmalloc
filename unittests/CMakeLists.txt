add_test(NAME malloc COMMAND malloc)

set_property(TEST malloc
   PROPERTY
   ENVIRONMENT LD_PRELOAD=${CMAKE_SOURCE_DIR}/build/alloc/liballoc.so)
