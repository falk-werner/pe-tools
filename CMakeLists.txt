cmake_minimum_required(VERSION 3.22)
project(pe-tools)

set(CMAKE_CXX_STANDARD 17)

add_library(pe-tools STATIC
    lib/pe_tools/file_reader.cpp
    lib/pe_tools/pe_file.cpp)
target_include_directories(pe-tools PUBLIC inc)
target_include_directories(pe-tools PRIVATE lib)

add_executable(pe-info src/pe_info.cpp)
target_link_libraries(pe-info PRIVATE pe-tools)

add_executable(pe-depends src/pe_depends.cpp)
target_link_libraries(pe-depends PRIVATE pe-tools)
