// Copyright (c) 2024 Falk Werner
// SPDX-License-Identifier: MIT

/// \file reader.c
/// \brief Implementation of pe_reader struct.

#include "pe/reader.h"
#include "pe/error.h"
#include "pe/buffer_reader.h"

#include "util/heap.h"
#include "util/str_stack.h"

#include <windows.h>

#define PE_SIGNATURE_OFFSET_OFFSET 0x3c
#define PE_SIGNATURE 0x4550
#define PE_COFF_HEADER_SIZE 20
#define PE_MAX_COFF_OPT_HEADER_SIZE 240
#define PE_MAGIC_PE32 0x10b
#define PE_MAGIC_PE32PLUS 0x20b
#define PE_SECTION_HEADER_SIZE 40
#define PE_IMPORT_DIR_ENTRY_SZE 20
#define PE_IMPORT_DIR_ENTRY_NAME_OFFSET 12
#define PE_MAX_FILENAME_SIZE 256

#define PE_GOOD 1
#define PE_BAD 0

struct pe_reader
{
    pe_u32 signature_offset;
    struct pe_coff_header header;
    struct pe_section_header * section_headers;
    struct str_stack * import_dlls;

};

static int read_at(HANDLE file, INT64 pos, void * buffer, int size)
{
    LARGE_INTEGER file_pos;
    file_pos.QuadPart = pos;

    BOOL result = SetFilePointerEx(file, file_pos, 0, FILE_BEGIN);
    if (!result) {
        return PE_BAD;
    }

    DWORD count = 0;
    result = ReadFile(file, buffer, size, &count, 0);
    if (!result) {
        return PE_BAD;
    }

    return (count == size);
}

static int read_u32_at(HANDLE file, INT64 pos, pe_u32 * data)
{
    unsigned char buffer[4];
    int result = read_at(file, pos, buffer, 4);

    if (result)
    {
        pe_u32 value = 0;
        for(int i = 0; i < 4; i++) {
            value <<= 8;
            value |= buffer[3 - i];
        }
        *data = value;
    }

    return result;
}

static int read_str_at(HANDLE file, INT64 pos, char* buffer, int size)
{
    LARGE_INTEGER file_pos;
    file_pos.QuadPart = pos;

    BOOL result = SetFilePointerEx(file, file_pos, 0, FILE_BEGIN);
    if (!result) {
        return PE_BAD;
    }

    char c = 'x';
    while ((size > 0) && (c != '\0'))
    {
        DWORD count = 0;
        result = ReadFile(file, &c, 1, &count, 0);
        if ((!result) || (count != 1)) {
            return PE_BAD;
        }

        *buffer = c;
        buffer++;
        size--;
    }

    return (c == '\0') ? PE_GOOD : PE_BAD;
}


static int read_pe_signature(HANDLE file, pe_u32 * signature_offset, struct pe_error * * err)
{
    if (!read_u32_at(file, PE_SIGNATURE_OFFSET_OFFSET, signature_offset)) {
        pe_error_set(err, "failed to read PE signature offset");
        return PE_BAD;
    }

    pe_u32 signature = 0;
    if (!read_u32_at(file, *signature_offset, &signature)) {
        pe_error_set(err, "failed to read PE signature");
        return PE_BAD;
    }

    if (signature != PE_SIGNATURE) {
        pe_error_set(err, "invalid PE signature");
        return PE_BAD;
    }

    return PE_GOOD;
}

static int read_coff_header(HANDLE file, pe_u32 signature_offset, struct pe_coff_header * header)
{
    pe_u8 data[PE_COFF_HEADER_SIZE];
    if (!read_at(file, signature_offset + 4, data, PE_COFF_HEADER_SIZE)) {
        return PE_BAD;        
    }

    struct pe_buffer_reader reader;
    pe_buffer_reader_init(&reader, data, PE_COFF_HEADER_SIZE);
    header->machine = pe_buffer_read_u16(&reader);
    header->number_of_sections = pe_buffer_read_u16(&reader);
    header->time_date_stamp = pe_buffer_read_u32(&reader);
    header->pointer_to_symbol_table = pe_buffer_read_u32(&reader);
    header->number_of_symbols = pe_buffer_read_u32(&reader);
    header->size_of_optional_header = pe_buffer_read_u16(&reader);
    header->characteristcs = pe_buffer_read_u16(&reader);

    return PE_GOOD;
}

static void read_pe32_header(struct pe_buffer_reader * reader, struct pe_coff_header * header)
{
    header->base_of_data = pe_buffer_read_u32(reader);
    header->image_base = pe_buffer_read_u32(reader);
    header->section_alignment = pe_buffer_read_u32(reader);
    header->file_alignment = pe_buffer_read_u32(reader);
    header->major_os_version = pe_buffer_read_u16(reader);
    header->minor_os_version = pe_buffer_read_u16(reader);
    header->major_image_version = pe_buffer_read_u16(reader);
    header->minor_image_version = pe_buffer_read_u16(reader);
    header->major_subsystem_version = pe_buffer_read_u16(reader);
    header->minor_subsystem_version = pe_buffer_read_u16(reader);
    header->win32_version_value = pe_buffer_read_u32(reader);
    header->size_of_image = pe_buffer_read_u32(reader);
    header->size_of_headers = pe_buffer_read_u32(reader);
    header->checksum = pe_buffer_read_u32(reader);
    header->subsystem = pe_buffer_read_u16(reader);
    header->dll_characteristics = pe_buffer_read_u16(reader);
    header->size_of_stack_reserve = pe_buffer_read_u32(reader);
    header->size_of_stack_commit = pe_buffer_read_u32(reader);
    header->size_of_heap_reserve = pe_buffer_read_u32(reader);
    header->size_of_heap_commit = pe_buffer_read_u32(reader);
    header->loader_flags = pe_buffer_read_u32(reader);
    header->number_of_rva_and_sizes = pe_buffer_read_u32(reader);
}

static void read_pe32plus_header(struct pe_buffer_reader * reader, struct pe_coff_header * header)
{
    header->base_of_data = 0;
    header->image_base = pe_buffer_read_u64(reader);
    header->section_alignment = pe_buffer_read_u32(reader);
    header->file_alignment = pe_buffer_read_u32(reader);
    header->major_os_version = pe_buffer_read_u16(reader);
    header->minor_os_version = pe_buffer_read_u16(reader);
    header->major_image_version = pe_buffer_read_u16(reader);
    header->minor_image_version = pe_buffer_read_u16(reader);
    header->major_subsystem_version = pe_buffer_read_u16(reader);
    header->minor_subsystem_version = pe_buffer_read_u16(reader);
    header->win32_version_value = pe_buffer_read_u32(reader);
    header->size_of_image = pe_buffer_read_u32(reader);
    header->size_of_headers = pe_buffer_read_u32(reader);
    header->checksum = pe_buffer_read_u32(reader);
    header->subsystem = pe_buffer_read_u16(reader);
    header->dll_characteristics = pe_buffer_read_u16(reader);
    header->size_of_stack_reserve = pe_buffer_read_u64(reader);
    header->size_of_stack_commit = pe_buffer_read_u64(reader);
    header->size_of_heap_reserve = pe_buffer_read_u64(reader);
    header->size_of_heap_commit = pe_buffer_read_u64(reader);
    header->loader_flags = pe_buffer_read_u32(reader);
    header->number_of_rva_and_sizes = pe_buffer_read_u32(reader);
}

static int read_coff_optional_header(HANDLE file, pe_u32 signature_offset, struct pe_coff_header * header, struct pe_error * * err)
{
    int const header_offset = signature_offset + 4 + PE_COFF_HEADER_SIZE;
    int const header_size = (header->size_of_optional_header < PE_MAX_COFF_OPT_HEADER_SIZE) ? header->size_of_optional_header : PE_MAX_COFF_OPT_HEADER_SIZE;
    pe_u8 data[PE_MAX_COFF_OPT_HEADER_SIZE];

    if (!read_at(file, header_offset, data, header_size)) {
        pe_error_set(err, "failed to read COFF optional header");
        return PE_BAD;
    }

    struct pe_buffer_reader reader;
    pe_buffer_reader_init(&reader, data, header_size);

    header->magic = pe_buffer_read_u16(&reader);
    header->major_linker_version = pe_buffer_read_u8(&reader);
    header->minor_linker_version = pe_buffer_read_u8(&reader);
    header->size_of_code = pe_buffer_read_u32(&reader);
    header->size_of_initialized_data = pe_buffer_read_u32(&reader);
    header->size_of_uninitialized_data = pe_buffer_read_u32(&reader);
    header->address_of_entry_point = pe_buffer_read_u32(&reader);
    header->base_of_code = pe_buffer_read_u32(&reader);

    switch (header->magic) {
        case PE_MAGIC_PE32:
            read_pe32_header(&reader, header);
            break;
        case PE_MAGIC_PE32PLUS:
            read_pe32plus_header(&reader, header);
            break;
        default:            
            pe_error_set(err, "invalid magic");
            return PE_BAD;
    }

    pe_u32 const count = header->number_of_rva_and_sizes;
    header->export_table.virtual_address = (count > 0) ? pe_buffer_read_u32(&reader) : 0;
    header->export_table.size = (count > 0) ? pe_buffer_read_u32(&reader) : 0;

    header->import_table.virtual_address = (count > 1) ? pe_buffer_read_u32(&reader) : 0;
    header->import_table.size = (count > 1) ? pe_buffer_read_u32(&reader) : 0;

    header->resource_table.virtual_address = (count > 2) ? pe_buffer_read_u32(&reader) : 0;
    header->resource_table.size = (count > 2) ? pe_buffer_read_u32(&reader) : 0;

    header->exception_table.virtual_address = (count > 3) ? pe_buffer_read_u32(&reader) : 0;
    header->exception_table.size = (count > 3) ? pe_buffer_read_u32(&reader) : 0;

    header->certificate_table.virtual_address = (count > 4) ? pe_buffer_read_u32(&reader) : 0;
    header->certificate_table.size = (count > 4) ? pe_buffer_read_u32(&reader) : 0;

    header->base_relocation_table.virtual_address = (count > 5) ? pe_buffer_read_u32(&reader) : 0;
    header->base_relocation_table.size = (count > 5) ? pe_buffer_read_u32(&reader) : 0;

    header->debug.virtual_address = (count > 6) ? pe_buffer_read_u32(&reader) : 0;
    header->debug.size = (count > 6) ? pe_buffer_read_u32(&reader) : 0;

    header->architecture.virtual_address = (count > 7) ? pe_buffer_read_u32(&reader) : 0;
    header->architecture.size = (count > 7) ? pe_buffer_read_u32(&reader) : 0;

    header->global_ptr.virtual_address = (count > 8) ? pe_buffer_read_u32(&reader) : 0;
    header->global_ptr.size = (count > 8) ? pe_buffer_read_u32(&reader) : 0;

    header->tls_table.virtual_address = (count > 9) ? pe_buffer_read_u32(&reader) : 0;
    header->tls_table.size = (count > 9) ? pe_buffer_read_u32(&reader) : 0;

    header->load_config_table.virtual_address = (count > 10) ? pe_buffer_read_u32(&reader) : 0;
    header->load_config_table.size = (count > 10) ? pe_buffer_read_u32(&reader) : 0;

    header->bound_import.virtual_address = (count > 11) ? pe_buffer_read_u32(&reader) : 0;
    header->bound_import.size = (count > 11) ? pe_buffer_read_u32(&reader) : 0;

    header->iat.virtual_address = (count > 12) ? pe_buffer_read_u32(&reader) : 0;
    header->iat.size = (count > 12) ? pe_buffer_read_u32(&reader) : 0;

    header->deplay_import_descriptor.virtual_address = (count > 13) ? pe_buffer_read_u32(&reader) : 0;
    header->deplay_import_descriptor.size = (count > 13) ? pe_buffer_read_u32(&reader) : 0;

    header->clr_runtime_header.virtual_address = (count > 14) ? pe_buffer_read_u32(&reader) : 0;
    header->clr_runtime_header.size = (count > 14) ? pe_buffer_read_u32(&reader) : 0;

    return PE_GOOD;
}

int read_section_headers(HANDLE file, struct pe_reader * pe)
{
    pe->section_headers = heap_alloc(sizeof(struct pe_section_header) * pe->header.number_of_sections);
    int const base = pe->signature_offset + 4 + PE_COFF_HEADER_SIZE + pe->header.size_of_optional_header;

    for(int i = 0; i < pe->header.number_of_sections; i++) {
        struct pe_section_header * header = &(pe->section_headers[i]);

        pe_u8 data[PE_SECTION_HEADER_SIZE];
        int offset = base + (i * PE_SECTION_HEADER_SIZE);

        if (!read_at(file, offset, data, PE_SECTION_HEADER_SIZE)) {
            return PE_BAD;
        }

        struct pe_buffer_reader reader;
        pe_buffer_reader_init(&reader, data, PE_SECTION_HEADER_SIZE);        

        pe_buffer_read_fixedstr(&reader, header->name, 8);
        header->virtual_size = pe_buffer_read_u32(&reader);
        header->virtual_address = pe_buffer_read_u32(&reader);
        header->size_of_raw_data = pe_buffer_read_u32(&reader);
        header->pointer_to_raw_data = pe_buffer_read_u32(&reader);
        header->pointer_to_relocations = pe_buffer_read_u32(&reader);
        header->pointer_to_line_numbers = pe_buffer_read_u32(&reader);
        header->number_of_relocations = pe_buffer_read_u32(&reader);
        header->number_of_line_numbers = pe_buffer_read_u32(&reader);
        header->characteristcs = pe_buffer_read_u32(&reader);
    }

    return PE_GOOD;
}

int rva_to_filepos(struct pe_reader const * pe, int address)
{
    struct pe_section_header const * section = 0;
    for(int i = 0; i < pe->header.number_of_sections; i++) {
        struct pe_section_header const * candidate = &(pe->section_headers[i]);

        if (candidate->virtual_size == 0) {
            continue;
        }

        if ((candidate->virtual_address <= address) && (address < (candidate->virtual_address + candidate->virtual_size))) {
            section = candidate;
            break;
        }
    }

    if (0 != section) {
        return address + section->pointer_to_raw_data - section->virtual_address;
    }

    return -1;
}

int read_import_dlls(HANDLE file, struct pe_reader * pe)
{
    pe->import_dlls = str_stack_new();
    if (pe->header.import_table.virtual_address == 0) {
        return PE_GOOD;
    }

    int base = rva_to_filepos(pe, pe->header.import_table.virtual_address);
    if (base < 0) {        
        return PE_BAD;
    }

    int done = 0;
    for(int i = 0; !done; i++) {
        int offset = base + (i * PE_IMPORT_DIR_ENTRY_SZE) + PE_IMPORT_DIR_ENTRY_NAME_OFFSET;

        pe_u32 name_rva = 0;
        if (!read_u32_at(file, offset, &name_rva)) {
            return PE_BAD;
        }

        if (name_rva != 0) {
            int name_pos = rva_to_filepos(pe, name_rva);
            if (name_pos < 0) {
                return PE_BAD;
            }

            char name[PE_MAX_FILENAME_SIZE];
            if (!read_str_at(file, name_pos, name, PE_MAX_FILENAME_SIZE)) {
                return PE_BAD;
            }

            str_stack_push(pe->import_dlls, name);
        }
        else {
            done = 1;
        }
    }

    return PE_GOOD;
}


struct pe_reader *
pe_reader_from_file(char const * filename, struct pe_error * * err)
{
    HANDLE file = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, 
        0, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, 0);
    if (file == INVALID_HANDLE_VALUE) {
        pe_error_set(err, "failed to open file");
        return PE_BAD;
    }

    struct pe_reader * reader = heap_alloc(sizeof(struct pe_reader));
    reader->section_headers = 0;
    reader->import_dlls = 0;

    if (!read_pe_signature(file, &reader->signature_offset, err)) {
        pe_reader_release(reader);
        CloseHandle(file);
        return PE_BAD;
    }

    if (!read_coff_header(file, reader->signature_offset, &(reader->header))) {
        pe_error_set(err, "failed to read COFF header");
        pe_reader_release(reader);
        CloseHandle(file);
        return PE_BAD;
    }

    if (!read_coff_optional_header(file, reader->signature_offset, &(reader->header), err)) {
        pe_reader_release(reader);
        CloseHandle(file);
        return PE_BAD;
    }

    if (!read_section_headers(file, reader)) {
        pe_error_set(err, "failed to read section headers");
        pe_reader_release(reader);
        CloseHandle(file);
        return PE_BAD;
    }

    if (!read_import_dlls(file, reader)) {
        pe_error_set(err, "failed to read import DLLs");
        pe_reader_release(reader);
        CloseHandle(file);
        return PE_BAD;
    }

    CloseHandle(file);
    return reader;
}

void pe_reader_release(struct pe_reader * reader)
{
    if (0 != reader->import_dlls) {
        str_stack_release(reader->import_dlls);
    }

    heap_free(reader->section_headers);
    heap_free(reader);
}

struct pe_coff_header const *
pe_reader_get_coff_header(struct pe_reader const * reader)
{
    return &(reader->header);
}

struct pe_section_header const *
pe_reader_get_section_header(struct pe_reader const * reader, int i)
{
    if ((0 <= i) && (i < reader->header.number_of_sections)) {
        return &(reader->section_headers[i]);
    }

    return 0;
}

int pe_reader_get_number_of_import_dlls(struct pe_reader const * reader)
{
    return reader->import_dlls->size;
}

char const *
pe_reader_get_import_dll(struct pe_reader const * reader, int i)
{
    if ((0 <= i) && (i < reader->import_dlls->size)) {
        return reader->import_dlls->data[i];
    }

    return 0;
}
