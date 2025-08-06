cmake_minimum_required(VERSION 3.28.2)
project(julmalloc C)

add_compile_options(-Wall -Wextra -Wpedantic -std=gnu23)

add_subdirectory(allocation)

add_executable(testalloc projects/allocation/testalloc.c)
target_link_libraries(testalloc alloc_functions)
