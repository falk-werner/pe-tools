// Copyright (c) 2024 Falk Werner
// SPDX-License-Identifier: MIT

/// \file section_header.h
/// \brief Defines pe_section_header struct.

#ifndef PE_SECTION_HEADER_H
#define PE_SECTION_HEADER_H

#include <pe/types.h>

#ifdef __cplusplus
extern "C"
{
#endif

/// @brief PE section header
struct pe_section_header
{
    char name[9];                       ///< Name of the section
    pe_u32 virtual_size;                ///< virtual size of the section
    pe_u32 virtual_address;             ///< virtual address of the section
    pe_u32 size_of_raw_data;            ///< size of the raw data of the section in the file
    pe_u32 pointer_to_raw_data;         ///< pointer to raw data in the file
    pe_u32 pointer_to_relocations;      ///< pointer to relocations
    pe_u32 pointer_to_line_numbers;     ///< pointer to COFF line numbers
    pe_u32 number_of_relocations;       ///< count of relocations
    pe_u32 number_of_line_numbers;      ///< count of COFF line numbers
    pe_u32 characteristcs;              ///< flags
};

#ifdef __cplusplus
}
#endif

#endif
