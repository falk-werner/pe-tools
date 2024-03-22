// Copyright (c) 2024 Falk Werner
// SPDX-License-Identifier: MIT

/// \file dll_finder.h
/// \brief Interface of pe_dll_finder.

#ifndef PE_DLL_FINDER_H
#define PE_DLL_FINDER_H

#ifdef __cplusplus
extern "C"
{
#endif

#define PE_DLL_FOUND 0          ///< DLL found
#define PE_DLL_KNOWN 1          ///< DLL is well known
#define PE_DLL_NOT_FOUND 2      ///< DLL not found

struct pe_dll_finder;

/// @brief Creates a new instance of a DLL finder.
/// @return Newly created instance of a DLL finder.
struct pe_dll_finder * pe_dll_finder_new(void);

/// @brief Released an instance of a DLL finder.
/// @param finder Pointer of DLL finder to release.
void pe_dll_finder_release(struct pe_dll_finder * finder);

/// @brief Find the specified DLL.
///
/// The buffer will only be filed, the DLL is found.
///
/// @param finder Pointer to the DLL finder.
/// @param name  Name of the DLL to find.
/// @param buffer Buffer where to store the file name of the found DLL.
/// @param buffer_size Size of the buffer.
/// @return PE_DLL_FOUND if the DLL was found, PE_DLL_KNOWN if the DLL is known, PE_DLL_NOT_FOUND otherwise
int pe_dll_finder_find(struct pe_dll_finder * finder, char const * name,
char * buffer, int buffer_size);

/// @brief Disabled DLL default seach order.
///
/// When default search order is disabled, only DLLs specified via pe_dll_finder_add_known_dll
/// and search paths specified via pe_dll_finder_add_seach_path will be used to find a DLL.
///
/// @param finder Pointer to the DLL finder.
/// @param disable 1 to disable default serach order, 0 to enable default search order 
void pe_dll_finder_disable_default_search_order(struct pe_dll_finder * finder, int disable);

/// @brief Add a DLL seach path.
/// @param finder Pointer to the DLL finder.
/// @param path Search Path to add.
void pe_dll_finder_add_search_path(struct pe_dll_finder * finder, char const * path);

/// @brief Add a known DLL.
/// @param finder Pointer to the DLL finder.
/// @param dll Name of the known DLL.
void pe_dll_finder_add_known_dll(struct pe_dll_finder * finder, char const * dll);

#ifdef __cplusplus
}
#endif

#endif