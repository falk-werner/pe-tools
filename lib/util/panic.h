// Copyright (c) 2024 Falk Werner
// SPDX-License-Identifier: MIT

/// \file panic.h
/// \brief Interface of panic function.

#ifndef UTIL_PANIC_H
#define UTIL_PANIC_H

/// @brief Prints the message to stderr and terminates the program
/// @param message Message to print.
void panic(char const * message);

#endif
