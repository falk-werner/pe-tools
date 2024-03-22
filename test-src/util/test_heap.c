// Copyright (c) 2024 Falk Werner
// SPDX-License-Identifier: MIT

#include "alltests.h"
#include "util/heap.h"

int test_heap_alloc_free(void)
{
    int result = 0;

    int const heap_size = heap_used_size();

    void * ptr = heap_alloc(42);
    if (ptr == 0) {
        test_set_message("alloc should never fail"); 
        return 1;
    }

    if (heap_size >= heap_used_size())
    {
        test_set_message("alloc should increase heap size");
        result = 1;
    }

    heap_free(ptr);
    if (heap_size != heap_used_size())
    {
        test_set_message("free should decrease heap size");
        result = 1;
    }

    return result;
}

int test_heap_free_nullptr(void)
{
    int const heap_size = heap_used_size();
    heap_free(0);
    return (heap_size == heap_used_size()) ? 0 : 1;
}

int test_heap_realloc(void)
{
    int result = 0;

    int const heap_size = heap_used_size();

    void * ptr = heap_alloc(42);
    if (ptr == 0) {
        test_set_message("alloc should never fail"); 
        return 1;
    }

    ptr = heap_realloc(ptr, 1337);
    if (ptr == 0) {
        test_set_message("realloc should never fail"); 
        result = 1;
    }

    heap_free(ptr);
    if ((result == 0) && (heap_size != heap_used_size()))
    {
        test_set_message("free should decrease heap size");
        result = 1;
    }

    return result;
}
