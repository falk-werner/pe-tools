// Copyright (c) 2024 Falk Werner
// SPDX-License-Identifier: MIT

/// \file reader.h
/// \brief Interface of pe_reader struct.

#ifndef PE_READER_H
#define PE_READER_H

#include <pe/coff_header.h>
#include <pe/section_header.h>
#include <pe/error.h>

#ifdef __cplusplus
extern "C"
{
#endif

struct pe_reader;

/// @brief Creates a new pe_reader instance.
///
/// @param filename Path of the .exe or .dll file to read.
/// @param err Pointer to store error message, may be 0.
/// @return Newly created pe_reader instance or 0 on failure.
struct pe_reader * 
pe_reader_from_file(char const * filename, struct pe_error * * err);

/// @brief Releases a reader instance.
///
/// Frees all resources associated with the reader.
///
/// @param reader Reader to release.
void pe_reader_release(struct pe_reader * reader);

/// @brief Returns the COFF header.
///
/// @param reader Pointer to the reader.
/// @return COFF header.
struct pe_coff_header const *
pe_reader_get_coff_header(struct pe_reader const * reader);

/// @brief Returns the i-th section header.
/// @param reader Pointer to the reader.
/// @param i Zero-based index of the section to return.
/// @return Section header or 0.
struct pe_section_header const *
pe_reader_get_section_header(struct pe_reader const * reader, int i);

/// @brief Returns the number of the imported DLLs.
/// @param reader Pointer to the reader.
/// @return Number of the imported DLLs.
int pe_reader_get_number_of_import_dlls(struct pe_reader const * reader);

/// @brief Returns the Name of the i-th import DLL.
/// @param reader Pointer to the reader.
/// @param i Zero-based index of the import DLL.
/// @return Name of the DLL or 0.
char const *
pe_reader_get_import_dll(struct pe_reader const * reader, int i);

#ifdef __cplusplus
}
#endif

#endif
