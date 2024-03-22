// Copyright (c) 2024 Falk Werner
// SPDX-License-Identifier: MIT

/// \file args.h
/// \brief Interface of command line splitter.

#ifndef UTIL_ARGS_H
#define UTIL_ARGS_H

#include "util/str_stack.h"

/// @brief Splits a command line string in separate arguments.
/// @param command_line Command line string.
/// @return Stack containing separate command line arguments.
struct str_stack * args_parse(char const * command_line);

#endif
