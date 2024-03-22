// Copyright (c) 2024 Falk Werner
// SPDX-License-Identifier: MIT

#include "alltests.h"
#include "util/str.h"

int test_str_len_empty_string(void)
{
    return (str_len("") == 0) ? 0 : 1; 
}

int test_str_len_nonempty_string(void)
{
    return (str_len("foo") == 3) ? 0 : 1;
}

int test_str_cp_empty_str(void)
{
    char target[1] = {'x'};
    char source[1] = {'\0'};

    str_cp(target, source);
    return (target[0] == '\0') ? 0 : 1;
}

int test_str_cp_nonempty_string(void)
{
    char target[80] = { '\0' };
    char source[] = "foobar";

    str_cp(target, source);
    return (str_eq(target, source)) ? 0 : 1;
}

int test_str_eq_all_empty(void)
{
    return (str_eq("", "")) ? 0 : 1;
}

int test_str_eq_equal(void)
{
    return (str_eq("foo", "foo")) ? 0 : 1;
}

int test_str_eq_not_equal(void)
{
    return (!str_eq("foo", "bar")) ? 0 : 1;
}

int test_str_eq_not_equals_first_empty(void)
{
    return (!str_eq("", "foo")) ? 0 : 1;
}

int test_str_eq_not_equals_second_empty(void)
{
    return (!str_eq("foo", "")) ? 0 : 1;
}

int test_u2str(void)
{
    int result = 0;

    if (!str_eq(u2str(0), "0")) {
        test_set_message("expected 0");
        result = 1;
    }

    if (!str_eq(u2str(42), "42")) {
        test_set_message("expected 42");
        result = 1;
    }

    if (!str_eq(u2str(1337), "1337")) {
        test_set_message("expected 1337");
        result = 1;
    }

    return result;
}

int test_u2hexstr(void)
{
    int result = 0;

    if (!str_eq(u2hexstr(0), "0")) {
        test_set_message("expected 0");
        result = 1;
    }

    if (!str_eq(u2hexstr(0x42), "42")) {
        test_set_message("expected 42");
        result = 1;
    }

    if (!str_eq(u2hexstr(0x1337), "1337")) {
        test_set_message("expected 1337");
        result = 1;
    }

    return result;
}

int test_str_eqi(void)
{
    return str_eqi("Hello", "helLO") ? 0 : 1;
}

int test_str_find(void)
{
    return (2 == str_find("Hello", 'l')) ? 0 : 1;
}

int test_str_find_fail(void)
{
    return (-1 == str_find("Hello", 'z')) ? 0 : 1;
}