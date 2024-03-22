// Copyright (c) 2024 Falk Werner
// SPDX-License-Identifier: MIT

/// \file error.h
/// \brief Interface of pe_error struct.

#ifndef PE_ERROR_H
#define PE_ERROR_H

#ifdef __cplusplus
extern "C"
{
#endif

/// @brief Error struct
struct pe_error;

/// @brief Sets an pe_error instance.
///
/// If error is. nothing will be done.
/// If error points to a value, a new error instance will be created.
///
/// @param err Pointer to a pointer of error struct.
/// @param message Error message.
void pe_error_set(struct pe_error * * err, char const * message);

/// @brief Releases an error instance.
/// @param err error to release.
void pe_error_release(struct pe_error * err);

/// @brief Return the error message.
/// @param err Pointer to the error.
/// @return Error Mmssage.
char const * pe_error_message(struct pe_error const * err);

#ifdef __cplusplus
}
#endif

#endif
