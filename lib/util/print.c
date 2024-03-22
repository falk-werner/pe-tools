// Copyright (c) 2024 Falk Werner
// SPDX-License-Identifier: MIT

#include "util/print.h"
#include "util/str.h"

#include <windows.h>

void print(char const * value)
{
    HANDLE stdout = GetStdHandle(STD_OUTPUT_HANDLE);
    WriteConsole(stdout, value, str_len(value), 0, 0);
}

void print_u(unsigned long long value)
{
    print(u2str(value));
}

void print_hex(unsigned long long value)
{
    print(u2hexstr(value));
}

void print_error(char const * message)
{
    HANDLE stderr = GetStdHandle(STD_ERROR_HANDLE);
    WriteConsole(stderr, "error: ", 7, 0, 0);
    WriteConsole(stderr, message, str_len(message), 0, 0);
    WriteConsole(stderr, "\n", 1, 0, 0);
}
