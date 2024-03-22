// Copyright (c) 2024 Falk Werner
// SPDX-License-Identifier: MIT

#ifndef PE_KNOWN_DLLS
#define PE_KNOWN_DLLS

/// \file known_dlls.h
/// \brief Interface to get known DLLs.

#include "util/str_stack.h"

/// @brief Adds known DLLs to the provided stack.
/// @param stack Stack to add known DLLs.
void pe_add_known_dlls(struct str_stack * stack);

#endif
