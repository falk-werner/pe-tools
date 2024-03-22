// Copyright (c) 2024 Falk Werner
// SPDX-License-Identifier: MIT

#include "alltests.h"
#include "util/args.h"
#include "util/str.h"

int test_args(void)
{
    int result = 0;

    struct str_stack * args = args_parse("some.exe foo bar \"foo bar\"");

    if (args->size != 4) {
        test_set_message("4 args expected");
        result = 1;
    }

    if ((result == 0) && (!str_eq(args->data[0], "some.exe"))) {
        test_set_message("arg[0] should equals some.exe");
        result = 1;
    }

    if ((result == 0) && (!str_eq(args->data[1], "foo"))) {
        test_set_message("arg[1] should equals foo");
        result = 1;
    }

    if ((result == 0) && (!str_eq(args->data[2], "bar"))) {
        test_set_message("arg[2] should equals bar");
        result = 1;
    }

    if ((result == 0) && (!str_eq(args->data[3], "foo bar"))) {
        test_set_message("arg[3] should equals >foo bar<");
        result = 1;
    }

    str_stack_release(args);
    return result;
}