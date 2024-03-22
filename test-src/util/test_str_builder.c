// Copyright (c) 2024 Falk Werner
// SPDX-License-Identifier: MIT

#include "alltests.h"
#include "util/str_builder.h"
#include "util/str.h"

int test_str_builder_new(void)
{
    int result = 0;

    struct str_builder * s = str_builder_new();
    if (!str_eq(s->data, "")) {
        test_set_message("s should be empty");
        result = 1;
    }

    if (s->size != 1) {
        test_set_message("s should be of length 1");
        result = 1;
    }

    str_builder_release(s);
    return result;
}


int test_str_builder_add_c(void)
{
    int result = 0;

    struct str_builder * s = str_builder_new();
    str_builder_add_c(s, 'f');
    str_builder_add_c(s, 'o');
    str_builder_add_c(s, 'o');
    str_builder_add_c(s, 'b');
    str_builder_add_c(s, 'a');
    str_builder_add_c(s, 'r');

    if (!str_eq(s->data, "foobar")) {
        test_set_message("s should be equal to foobar");
        result = 1;
    }

    str_builder_release(s);
    return result;
}

int test_str_builder_add_c_multi(void)
{
    int result = 0;
    struct str_builder * s = str_builder_new();
    for(char c = 0; c < 120; c++) {
        str_builder_add_c(s, c);
    }

    for(char c = 0; c < 120; c++) {
        if (s->data[c] != c) {
            result = 1;
            break;
        }
    }

    if ((result == 0) && (s->data[120] != '\0')) {
        test_set_message("last char should be 0");
        result = 1;
    }

    str_builder_release(s);
    return result;
}

int test_str_builder_reset(void)
{
    int result = 0;
    struct str_builder * s = str_builder_new();
    str_builder_add_c(s, 'f');
    str_builder_add_c(s, 'o');
    str_builder_add_c(s, 'o');

    if (!str_eq(s->data, "foo")) {
        test_set_message("s should be equal to foo");
        result = 1;
    }

    str_builder_reset(s);
    str_builder_add_c(s, 'b');
    str_builder_add_c(s, 'a');
    str_builder_add_c(s, 'r');

    if (!str_eq(s->data, "bar")) {
        test_set_message("s should be equal to bar");
        result = 1;
    }

    str_builder_release(s);
    return result;
}