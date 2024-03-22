// Copyright (c) 2024 Falk Werner
// SPDX-License-Identifier: MIT

#include "pe/error.h"
#include "util/str.h"

int test_error(void)
{
    int result = 0;

    struct pe_error * err;
    pe_error_set(&err, "foo");
    if (!str_eq(pe_error_message(err), "foo")) {
        result = 1;
    }

    pe_error_release(err);
    return result;
}

int test_error_null(void)
{
    pe_error_set(0, "foo");
    return 0;
}
