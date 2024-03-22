// Copyright (c) 2024 Falk Werner
// SPDX-License-Identifier: MIT

/// \file coff_header.h
/// \brief Defines pe_coff_header struct.

#ifndef PE_COFF_HEADER_H
#define PE_COFF_HEADER_H

#include <pe/types.h>

#ifdef __cplusplus
extern "C"
{
#endif

/// @brief PE data directory.
struct pe_data_directory
{
    pe_u32 virtual_address; ///< virtual address
    pe_u32 size;            ///< size
};

/// @brief COFF header and optional header.
struct pe_coff_header
{
    pe_u16 machine;                     ///< target platform, such as x86-64 or AArch64
    pe_u16 number_of_sections;          ///< number of sections in the file
    pe_u32 time_date_stamp;             ///< timestamp
    pe_u32 pointer_to_symbol_table;     ///< pointer to the symbol table
    pe_u32 number_of_symbols;           ///< number of symbols in the symbol table
    pe_u16 size_of_optional_header;     ///< size of the optional COFF header in bytes
    pe_u16 characteristcs;              ///< characteristics

    pe_u16 magic;                       ///< magic, 0x10b for PE32, 0x20b for PE32+
    pe_u8 major_linker_version;         ///< major version of the linker
    pe_u8 minor_linker_version;         ///< minor version of the linker
    pe_u32 size_of_code;                ///< total size of the code
    pe_u32 size_of_initialized_data;    ///< total size of initilized data
    pe_u32 size_of_uninitialized_data;  ///< total size of uninitialized data
    pe_u32 address_of_entry_point;      ///< address of the entry point
    pe_u32 base_of_code;                ///< base of code

    pe_u32 base_of_data;                ///< base of data, PE32 only

    pe_u64 image_base;                  ///< image base
    pe_u32 section_alignment;           ///< alignment of secrions
    pe_u32 file_alignment;              ///< alignment of the file
    pe_u16 major_os_version;            ///< major operating system version
    pe_u16 minor_os_version;            ///< minor operating system version
    pe_u16 major_image_version;         ///< major image version
    pe_u16 minor_image_version;         ///< minor image version
    pe_u16 major_subsystem_version;     ///< major subsystem version
    pe_u16 minor_subsystem_version;     ///< minor subsystem version
    pe_u32 win32_version_value;         ///< win32 version, always 0
    pe_u32 size_of_image;               ///< size of the image
    pe_u32 size_of_headers;             ///< size of headers
    pe_u32 checksum;                    ///< file checksum
    pe_u16 subsystem;                   ///< subsystem, e.g. GUI or TUI
    pe_u16 dll_characteristics;         ///< DLL characteristics
    pe_u64 size_of_stack_reserve;       ///< size of stack reserve
    pe_u64 size_of_stack_commit;        ///< size of stack commit
    pe_u64 size_of_heap_reserve;        ///< size of heap reserve
    pe_u64 size_of_heap_commit;         ///< size of heap commit
    pe_u32 loader_flags;                ///< loader flags
    pe_u32 number_of_rva_and_sizes;     ///< count of data directories

    struct pe_data_directory export_table;              ///< export tabble
    struct pe_data_directory import_table;              ///< import table
    struct pe_data_directory resource_table;            ///< resource table
    struct pe_data_directory exception_table;           ///< exception table
    struct pe_data_directory certificate_table;         ///< certificate table
    struct pe_data_directory base_relocation_table;     ///< base relocation table
    struct pe_data_directory debug;                     ///< debug
    struct pe_data_directory architecture;              ///< architecture
    struct pe_data_directory global_ptr;                ///< global ptr
    struct pe_data_directory tls_table;                 ///< TLS table
    struct pe_data_directory load_config_table;         ///< load config table
    struct pe_data_directory bound_import;              ///< bound import
    struct pe_data_directory iat;                       ///< IAT
    struct pe_data_directory deplay_import_descriptor;  ///< delay import descriptor
    struct pe_data_directory clr_runtime_header;        ///< CLR runtime header
};

#ifdef __cplusplus
}
#endif


#endif
