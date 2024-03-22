// Copyright (c) 2024 Falk Werner
// SPDX-License-Identifier: MIT

/// \file str_builder.h
/// \brief Interface of string builder.

#ifndef UTIL_STR_BUILDER_H
#define UTIL_STR_BUILDER_H

/// @brief String builder.
struct str_builder
{
    char * data;            ///< Buffer of text data.
    unsigned int size;      ///< Current count of characters in the buffer.
    unsigned int capacity;  ///< Total capacity of the buffer.
};

/// @brief Creates a new string builder.
/// @return Newly created string builder.
struct str_builder * str_builder_new(void);

/// @brief Releases a string builder.
/// @param builder String builder to release.
void str_builder_release(struct str_builder * builder);

/// @brief Append a single character to the string builder.
///
/// The capacity to the builder will be increased on demand.
///
/// @param builder Pointer to the string builder.
/// @param c Character to append.
void str_builder_add_c(struct str_builder * builder, char c);

/// @brief Resets the string builder.
///
/// Sets the size to 0.
///
/// @param builder Pointer to the builder.
void str_builder_reset(struct str_builder * builder);


#endif
