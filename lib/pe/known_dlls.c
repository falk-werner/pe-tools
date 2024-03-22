// Copyright (c) 2024 Falk Werner
// SPDX-License-Identifier: MIT

/// \file known_dlls.c
/// \brief Implementation of determination knwon DLLs.

#include "pe/known_dlls.h"
#include <windows.h>

#define MAX_STR_LEN 256

void pe_add_known_dlls(struct str_stack * stack)
{
    HKEY key;
    LSTATUS status = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\Session Manager\\KnownDLLs", 0, KEY_READ, &key);
    if (status != ERROR_SUCCESS) {
        return;
    }

    char name[MAX_STR_LEN];
    DWORD name_len = MAX_STR_LEN;
    DWORD type;
    char value[MAX_STR_LEN];
    DWORD value_len = MAX_STR_LEN;

    DWORD i = 0;
    status = RegEnumValue(key, i, name, &name_len, 0, &type, (LPBYTE) value, &value_len);
    while (status == ERROR_SUCCESS) {
        if (type == REG_SZ) {
            str_stack_push(stack, value);
        }
        i++;
        name_len = MAX_STR_LEN;
        value_len = MAX_STR_LEN;
        status = RegEnumValue(key, i, name, &name_len, 0, &type, (LPBYTE) value, &value_len);
    }

    RegCloseKey(key);
}
