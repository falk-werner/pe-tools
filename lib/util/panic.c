// Copyright (c) 2024 Falk Werner
// SPDX-License-Identifier: MIT

#include "util/panic.h"
#include "util/print.h"

#include <windows.h>

void panic(char const * message)
{
    print_error(message);
    ExitProcess(1);
}
