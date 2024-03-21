cmake_minimum_required(VERSION 3.22)
project(pe-tools)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 99)

add_library(pe-tools STATIC
    lib/pe_tools/file_reader.cpp
    lib/pe_tools/pe_file.cpp
    lib/pe_tools/known_dlls.cpp
    lib/pe_tools/dll_finder.cpp)
target_include_directories(pe-tools PUBLIC inc)
target_include_directories(pe-tools PRIVATE lib)

add_executable(pe-depends src/pe_depends.cpp)
target_link_libraries(pe-depends PRIVATE pe-tools)

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

add_executable(pe-depends-static 
    src/pe_depends_static/main.c
    src/pe_depends_static/file_reader.c
    )
target_link_libraries(pe-depends-static PRIVATE util)
target_link_options(pe-depends-static PRIVATE -nostdlib -nodefaultlibs -lkernel32)

add_executable(pe-info src/pe_info.c)
target_link_libraries(pe-info PRIVATE pe util)
target_link_options(pe-info PRIVATE -nostdlib -nodefaultlibs -lkernel32)

add_executable(dll-finder src/dll_finder.c)
target_link_libraries(dll-finder PRIVATE pe util)
target_link_options(dll-finder PRIVATE -nostdlib -nodefaultlibs -lkernel32)


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
target_link_libraries(alltests PRIVATE test)
target_link_libraries(alltests PRIVATE util)
target_link_libraries(alltests PRIVATE pe)
target_link_options(alltests PRIVATE -nostdlib -nodefaultlibs -lkernel32)

