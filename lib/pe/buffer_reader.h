// Copyright (c) 2024 Falk Werner
// SPDX-License-Identifier: MIT

/// \file buffer_reader.h
/// \brief Interface of pe_buffer_reader.

#ifndef PE_BUFFER_READER_H
#define PE_BUFFER_READER_H

#include "pe/types.h"

/// @brief Struct to read a buffer of data.
struct pe_buffer_reader
{
    pe_u8 const * data;     ///< Pointer to data to read.
    int pos;                ///< Current position within the data.
    int size;               ///< Total size of the data in the buffer in bytes.
};

/// @brief Initializes a reder.
/// @param reader Reader to initialize.
/// @param data   Pointer to the data to read.
/// @param size   Totoal size of teh data in bytes.
void pe_buffer_reader_init(struct pe_buffer_reader * reader, pe_u8 const * data, int size);

/// @brief Reads a single byte from the buffer.
///
/// Return 0, if no more data available.
///
/// @param reader Reader to read from.
/// @return Next byte to read or 0 if the reader is exhausted.
pe_u8 pe_buffer_read_u8(struct pe_buffer_reader * reader);

/// @brief Reads an unsigned 16-bit value from the buffer.
///
/// Return 0, if no more data available.
///
/// @param reader Reader to read from.
/// @return Next value to read or 0 if the reader is exhausted.
pe_u16 pe_buffer_read_u16(struct pe_buffer_reader * reader);

/// @brief Reads an unsigned 32-bit value from the buffer.
///
/// Return 0, if no more data available.
///
/// @param reader Reader to read from.
/// @return Next value to read or 0 if the reader is exhausted.
pe_u32 pe_buffer_read_u32(struct pe_buffer_reader * reader);

/// @brief Read an unsigned 64-bit value from the buffer.
///
/// Return 0, if no more data available.
///
/// @param reader Reader to read from.
/// @return Next value to read or 0 if the reader is exhausted.
pe_u64 pe_buffer_read_u64(struct pe_buffer_reader * reader);

/// @brief Reads a fixed size string from the buffer.
///
/// Reads an empty string, if the buffer is exhausted.
///
/// @param reader Reader to read from.
/// @param buffer Buffer to store the read string to.
/// @param size Size of the string in bytes.
void pe_buffer_read_fixedstr(struct pe_buffer_reader * reader, char * buffer, int size);

#endif
