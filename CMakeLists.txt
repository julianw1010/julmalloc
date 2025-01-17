cmake_minimum_required(VERSION 3.28.2)
project(julmalloc C)

add_compile_options(-Wall -Wextra -Wpedantic -std=gnu23 -lm)

add_subdirectory(alloc)

add_executable(testalloc projects/alloctests.c)
target_link_libraries(testalloc methods tableopts)
