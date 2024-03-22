// Copyright (c) 2024 Falk Werner
// SPDX-License-Identifier: MIT

#ifndef UTIL_HEAP_H
#define UTIL_HEAP_H

void * heap_alloc(int size);

void heap_free(void * ptr);

void * heap_realloc(void * ptr, int new_size);

int heap_used_size(void);

#endif
