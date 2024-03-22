// Copyright (c) 2024 Falk Werner
// SPDX-License-Identifier: MIT

/// \file print.h
/// \brief Interface of text output.

#ifndef UTIL_PRINT_H
#define UTIL_PRINT_H

/// @brief Prints a text to stdout.
/// @param value Text to print.
void print(char const * value);

/// @brief Prints an unsigned number to stdout.
/// @param value Number to print.
void print_u(unsigned long long value);

/// @brief Prints an unsigned number as hex value to stdout.
/// @param value Number to print.
void print_hex(unsigned long long value);

/// @brief Prints an error message stderr.
///
/// The message will be prefixed by "error: " and suffixed by "\n".
///
/// @param message Message to print.
void print_error(char const * message);

#endif
