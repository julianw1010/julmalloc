add_compile_options(-fPIC)

add_library(alloc sources/storage.c sources/methods.c sources/strats.c sources/tableio.c sources/tableopts.c)

target_link_libraries(alloc m)