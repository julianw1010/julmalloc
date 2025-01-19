cmake_minimum_required(VERSION 3.28.2)

project(julmalloc LANGUAGES C VERSION 1.0.0 DESCRIPTION "julmalloc")

add_compile_options(-Wall -Wextra -Wpedantic -std=gnu23 -lm)
include(CTest)

install(CODE "
  message(STATUS \"PROJECT_SOURCE_DIR: ${PROJECT_SOURCE_DIR}\")
  message(STATUS \"CMAKE_BINARY_DIR: ${CMAKE_BINARY_DIR}\")
  message(STATUS \"CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}\")
")

add_subdirectory(alloc)
add_subdirectory(unittests)
add_subdirectory(programs)

add_executable(alloctests unittests/alloctests.c)
target_link_libraries(alloctests alloc)

add_test(NAME alloctests COMMAND alloctests)
set_property(TEST alloctests
   PROPERTY
   ENVIRONMENT LD_PRELOAD=${CMAKE_CURRENT_BINARY_DIR}/lib/liballoc.so)
