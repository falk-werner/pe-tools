cmake_minimum_required(VERSION 3.22)
project(pe-tools VERSION 1.0.0)

set(CMAKE_C_STANDARD 99)

add_compile_options(-nostdlib -nodefaultlibs)
add_link_options(-nostdlib -nodefaultlibs -lkernel32)

add_library(util STATIC
    lib/util/print.c
    lib/util/str.c
    lib/util/str_builder.c
    lib/util/heap.c
    lib/util/str_stack.c
    lib/util/panic.c
    lib/util/args.c
    lib/util/cli.c
)
target_include_directories(util PUBLIC lib)

add_library(pe STATIC
    lib/pe/reader.c
    lib/pe/error.c
    lib/pe/buffer_reader.c
    lib/pe/dll_finder.c
    lib/pe/known_dlls.c
)
target_include_directories(pe PUBLIC inc)
target_include_directories(pe PRIVATE lib)

configure_file(src/version.h.in version.h @ONLY)

add_executable(pe-info src/pe_info.c)
target_link_libraries(pe-info PRIVATE pe util)
target_include_directories(pe-info PRIVATE "${CMAKE_CURRENT_BINARY_DIR}")

add_executable(dll-finder src/dll_finder.c)
target_link_libraries(dll-finder PRIVATE pe util)
target_include_directories(dll-finder PRIVATE "${CMAKE_CURRENT_BINARY_DIR}")

add_executable(pe-depends src/pe_depends.c)
target_link_libraries(pe-depends PRIVATE pe util)
target_include_directories(pe-depends PRIVATE "${CMAKE_CURRENT_BINARY_DIR}")

add_library(test STATIC
    lib/test/test.c)
target_include_directories(test PUBLIC lib)

add_executable(alltests
    test-src/alltests.c
    test-src/util/test_str.c
    test-src/util/test_heap.c
    test-src/util/test_str_builder.c
    test-src/util/test_args.c
    test-src/util/test_cli.c
    test-src/pe/test_error.c
    test-src/pe/test_buffer_reader.c
)
target_include_directories(alltests PRIVATE test-src)
target_link_libraries(alltests PRIVATE test util pe)
