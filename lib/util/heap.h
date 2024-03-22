// Copyright (c) 2024 Falk Werner
// SPDX-License-Identifier: MIT

/// \file heap.h
/// \brief Interface of memory allocation.

#ifndef UTIL_HEAP_H
#define UTIL_HEAP_H

/// @brief Allocates memory.
///
/// The program will terminate with a panic if allocation fails.
/// Therefore no return value checking is required.
///
/// @param size Size of the memory block to allocation.
/// @return Pointer to the newly allocated memory block.
void * heap_alloc(int size);

/// @brief Release a previously allocated memory block.
/// @param ptr Pointer to the memory block to release.
void heap_free(void * ptr);

/// @brief Re-allocated a memory block.
/// @param ptr Pointer to the previously allocated block.
/// @param new_size New size of the the memory block.
/// @return Pointer to the re-allocated memory block.
void * heap_realloc(void * ptr, int new_size);

/// @brief Returns the total size of currently allocated memory blocks.
/// @return Totoal size of currently allocated memory blocks.
int heap_used_size(void);

#endif
