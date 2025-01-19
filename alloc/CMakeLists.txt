add_compile_options(-fPIC)

add_library(alloc SHARED sources/storage.c sources/methods.c sources/strats.c sources/tableio.c sources/tableopts.c)
set_target_properties(alloc PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(alloc PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})

target_link_libraries(alloc m)