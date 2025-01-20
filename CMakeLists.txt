cmake_minimum_required(VERSION 3.28.2)

project(julmalloc LANGUAGES C VERSION 1.0.0 DESCRIPTION "julmalloc")

add_compile_options(-Wall -Wextra -Wno-unused-parameter -Wno-unused-function -Wno-unused-variable -std=gnu23 -lm)
include(CTest)

install(CODE "
  message(STATUS \"PROJECT_SOURCE_DIR: ${PROJECT_SOURCE_DIR}\")
  message(STATUS \"CMAKE_BINARY_DIR: ${CMAKE_BINARY_DIR}\")
  message(STATUS \"CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}\")
")

set(CMAKE_C_CLANG_TIDY "clang-tidy")

add_subdirectory(alloc)
add_subdirectory(unittests)
add_subdirectory(programs)

add_executable(malloc unittests/alloc/malloc.c)
target_link_libraries(malloc alloc)


add_test(NAME alloctests COMMAND alloctests)
set_property(TEST alloctests
   PROPERTY
   ENVIRONMENT LD_PRELOAD=${CMAKE_CURRENT_BINARY_DIR}/lib/liballoc.so)

# Additional targets to perform clang-format/clang-tidy
# Get all project files
file(GLOB_RECURSE
    ALL_C_SOURCE_FILES
    *.[chi]
)

# Adding clang-format target if executable is found
find_program(CLANG_FORMAT "clang-format")
if(CLANG_FORMAT)
    add_custom_target(
        clang-format
        COMMAND clang-format
        -i
        -style=file
        ${ALL_C_SOURCE_FILES}
    )
endif()
