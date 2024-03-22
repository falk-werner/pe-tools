// Copyright (c) 2024 Falk Werner
// SPDX-License-Identifier: MIT

#ifndef PE_SECTION_HEADER_H
#define PE_SECTION_HEADER_H

#include <pe/types.h>

#ifdef __cplusplus
extern "C"
{
#endif

struct pe_section_header
{
    char name[9];
    pe_u32 virtual_size;
    pe_u32 virtual_address;
    pe_u32 size_of_raw_data;
    pe_u32 pointer_to_raw_data;
    pe_u32 pointer_to_relocations;
    pe_u32 pointer_to_line_numbers;
    pe_u32 number_of_relocations;
    pe_u32 number_of_line_numbers;
    pe_u32 characteristcs;
};

#ifdef __cplusplus
}
#endif

#endif
