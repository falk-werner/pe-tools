// Copyright (c) 2024 Falk Werner
// SPDX-License-Identifier: MIT

#ifndef PE_COFF_HEADER_H
#define PE_COFF_HEADER_H

#include <pe/types.h>

#ifdef __cplusplus
extern "C"
{
#endif

struct pe_data_directory
{
    pe_u32 virtual_address;
    pe_u32 size;
};

struct pe_coff_header
{
    pe_u16 machine;
    pe_u16 number_of_sections;
    pe_u32 time_date_stamp;
    pe_u32 pointer_to_symbol_table;
    pe_u32 number_of_symbols;
    pe_u16 size_of_optional_header;
    pe_u16 characteristcs;

    pe_u16 magic;
    pe_u8 major_linker_version;
    pe_u8 minor_linker_version;
    pe_u32 size_of_code;
    pe_u32 size_of_initialized_data;
    pe_u32 size_of_uninitialized_data;
    pe_u32 address_of_entry_point;
    pe_u32 base_of_code;

    pe_u32 base_of_data;

    pe_u64 image_base;
    pe_u32 section_alignment;
    pe_u32 file_alignment;
    pe_u16 major_os_version;
    pe_u16 minor_os_version;
    pe_u16 major_image_version;
    pe_u16 minor_image_version;
    pe_u16 major_subsystem_version;
    pe_u16 minor_subsystem_version;
    pe_u32 win32_version_value;
    pe_u32 size_of_image;
    pe_u32 size_of_headers;
    pe_u32 checksum;
    pe_u16 subsystem;
    pe_u16 dll_characteristics;
    pe_u64 size_of_stack_reserve;
    pe_u64 size_of_stack_commit;
    pe_u64 size_of_heap_reserve;
    pe_u64 size_of_heap_commit;
    pe_u32 loader_flags;
    pe_u32 number_of_rva_and_sizes;

    struct pe_data_directory export_table;
    struct pe_data_directory import_table;
    struct pe_data_directory resource_table;
    struct pe_data_directory exception_table;
    struct pe_data_directory certificate_table;
    struct pe_data_directory base_relocation_table;
    struct pe_data_directory debug;
    struct pe_data_directory architecture;
    struct pe_data_directory global_ptr;
    struct pe_data_directory tls_table;
    struct pe_data_directory load_config_table;
    struct pe_data_directory bound_import;
    struct pe_data_directory iat;
    struct pe_data_directory deplay_import_descriptor;
    struct pe_data_directory clr_runtime_header;
};

#ifdef __cplusplus
}
#endif


#endif
