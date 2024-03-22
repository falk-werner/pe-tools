// Copyright (c) 2024 Falk Werner
// SPDX-License-Identifier: MIT

/// \file dll_finder.c
/// \brief Implementation of pe_dll_finder.

#include "pe/dll_finder.h"
#include "pe/known_dlls.h"

#include "util/heap.h"
#include "util/str_stack.h"
#include "util/str.h"

#include <windows.h>

#define BUFFER_SIZE 1024
#define MAX_ENV_SIZE (32 * 1024)

static int write_filename(char const * directory, char const * filename, char * buffer, int buffer_size)
{
    int const dir_len = str_len(directory);
    int const name_len = str_len(filename);

    if ((dir_len + 2 + name_len) >= buffer_size) { 
        // name too lang
        buffer[0] = '\0';
        return 0;
    }

    for(int i = 0; i < dir_len; i++) {
        buffer[i] = directory[i];
    }
    buffer[dir_len] = '\\';
    for(int i = 0; i < name_len; i++) {
        buffer[dir_len + 1 + i] = filename[i];
    }
    buffer[dir_len + 1 + name_len] = '\0';

    return 1;
}

static int file_exists(char const * directory, char const * filename, char * buffer, int buffer_size)
{
    if (!write_filename(directory, filename, buffer, buffer_size)) {
        // name too lang
        return 0;
    }

    WIN32_FIND_DATAA data;
    HANDLE h = FindFirstFileA(buffer, &data);
    int const result = (h != INVALID_HANDLE_VALUE);
    if (h != INVALID_HANDLE_VALUE) {
        FindClose(h);
    }

    return result;
}

struct pe_dll_finder
{
    struct str_stack * known_dlls;
    struct str_stack * extra_known_dlls;
    struct str_stack * paths;
    struct str_stack * extra_paths;
    int use_default_search_order;
};

struct pe_dll_finder * pe_dll_finder_new(void)
{
    struct pe_dll_finder * finder = heap_alloc(sizeof(struct pe_dll_finder));
    finder->known_dlls = str_stack_new();
    finder->extra_known_dlls = str_stack_new();
    finder->paths = str_stack_new();
    finder->extra_paths = str_stack_new();
    finder->use_default_search_order = 1;

    pe_add_known_dlls(finder->known_dlls);

    char buffer[BUFFER_SIZE];
    if (0 != GetSystemDirectory(buffer, BUFFER_SIZE)) {
        str_stack_push(finder->paths, buffer);
    }
    if (0 != GetWindowsDirectory(buffer, BUFFER_SIZE)) {
        str_stack_push(finder->paths, buffer);
    }

    static char path[MAX_ENV_SIZE];
    if (0 != GetEnvironmentVariable("PATH", path, MAX_ENV_SIZE)) {
        char * p = path;
        int i = str_find(p, ';');
        while (i >= 0) {
            p[i] = '\0';
            if (p[0] != '\0') {
                str_stack_push(finder->paths, p);
            }
            p = &p[i + 1];
            i = str_find(p, ';');
        }
        if (p[0] != '\0') {
            str_stack_push(finder->paths, p);
        }
    }

    return finder;
}

void pe_dll_finder_release(struct pe_dll_finder * finder)
{
    heap_free(finder->extra_paths);
    heap_free(finder->paths);
    heap_free(finder->extra_known_dlls);
    heap_free(finder->known_dlls);
}

int pe_dll_finder_find(struct pe_dll_finder * finder, char const * name,
char * buffer, int buffer_size)
{
    // check default known DLLs
    if (finder->use_default_search_order) {
        for(int i = 0; i < finder->known_dlls->size; i++) {
            char const * known = finder->known_dlls->data[i];
            if (str_eqi(name, known)) {
                return PE_DLL_KNOWN;
            }
        }
    }

    // check extra known DLLs
    for(int i = 0; i < finder->extra_known_dlls->size; i++) {
        char const * known = finder->extra_known_dlls->data[i];
        if (str_eqi(name, known)) {
            return PE_DLL_KNOWN;
        }
    }

    // search default paths
    if (finder->use_default_search_order) {
        if (file_exists(".", name, buffer, buffer_size)) {
            return PE_DLL_FOUND;
        }

        for(int i = 0; i < finder->paths->size; i++) {
            char const * dir = finder->paths->data[i];
            if (file_exists(dir, name, buffer, buffer_size)) {
                return PE_DLL_FOUND;
            }
        }
    }

    // search extra paths
    for(int i = 0; i < finder->extra_paths->size; i++) {
        char const * dir = finder->extra_paths->data[i];
        if (file_exists(dir, name, buffer, buffer_size)) {
            return PE_DLL_FOUND;
        }
    }

    return PE_DLL_NOT_FOUND;
}

void pe_dll_finder_disable_default_search_order(struct pe_dll_finder * finder, int disable)
{
    finder->use_default_search_order = !disable;
}

void pe_dll_finder_add_search_path(struct pe_dll_finder * finder, char const * path)
{
    str_stack_push(finder->extra_paths, path);
}

void pe_dll_finder_add_known_dll(struct pe_dll_finder * finder, char const * dll)
{
    str_stack_push(finder->extra_known_dlls, dll);
}
