#include "util/heap.h"
#include "util/panic.h"

#include <windows.h>

static int g_heap_size = 0;

void * heap_alloc(int size)
{
    HANDLE heap = GetProcessHeap();
    void * ptr = HeapAlloc(heap, 0, size);
    if (ptr == 0)
    {
        panic("heap_alloc: failed to alloc memory");
    }

    g_heap_size += size;
    return ptr;
}

void heap_free(void * ptr)
{
    if (0 != ptr)
    {
        HANDLE heap = GetProcessHeap();
        SIZE_T size = HeapSize(heap, 0, ptr);
        if (size == ((SIZE_T) -1))
        {
            panic("heap_free: unknown memory");
        }

        HeapFree(heap, 0, ptr);
        g_heap_size -= (int) size;
    }
}

void * heap_realloc(void * ptr, int new_size)
{
    HANDLE heap = GetProcessHeap();
    SIZE_T size = HeapSize(heap, 0, ptr);
    if (size == ((SIZE_T) -1))
    {
        panic("heap_free: unknown memory");
    }

    ptr = HeapReAlloc(heap, 0, ptr, new_size);
    if (ptr == 0)
    {
        panic("heap_realloc: failed to alloc memory");
    }

    g_heap_size += new_size - size;
    return ptr;
}

int heap_used_size(void)
{
    return g_heap_size;
}
