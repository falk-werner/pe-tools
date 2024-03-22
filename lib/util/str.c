// Copyright (c) 2024 Falk Werner
// SPDX-License-Identifier: MIT

#include "util/str.h"
#include "util/heap.h"

static char to_lower(char c)
{
    if (('A' <= c) && (c <= 'Z')) {
        return c - 'A' + 'a';
    }

    return c;
}

unsigned int str_len(char const * value)
{
    unsigned int result = 0;
    while (value[result] != '\0') { result++; }
    return result;
}

void str_cp(char * target, char const * source)
{
    while (*source != '\0') {
        *target = *source;
        target++;
        source++;
    }
    *target = *source;
}

int str_eq(char const * a, char const * b)
{
    while ((*a != '\0') && (*a == *b)) {
        a++;
        b++;
    }

    return (((*a) - (*b)) == 0);
}

int str_eqi(char const * a, char const * b)
{
    while ((*a != '\0') && (to_lower(*a) == to_lower(*b))) {
        a++;
        b++;
    }

    return ((to_lower(*a) - to_lower(*b)) == 0);

}

int str_find(char const * value, char c)
{
    for (int i = 0; value[i] != '\0'; i++) {
        if (value[i] == c) {
            return i;
        }
    }

    return -1;
}


char * str_dup(char const * s)
{
    unsigned int len = str_len(s);
    char * result = heap_alloc(len + 1);
    str_cp(result, s);

    return result;
}

char * u2str(unsigned long long value)
{
    static char buffer[32];
    int pos = 31;
    buffer[pos] = '\0';
    do {
        pos--;
        buffer[pos] = '0' + ((char) (value % 10));
        value /= 10;
    } while (value > 0);

    return &(buffer[pos]);
}

char * u2hexstr(unsigned long long value)
{
    static char buffer[32];
    int pos = 31;
    buffer[pos] = '\0';
    do {
        char const c = (char) (value % 16);
        pos--;
        buffer[pos] =  (c < 10) ? '0' + c : 'A' + c - 10;
        value /= 16;
    } while (value > 0);

    return &(buffer[pos]);
}

int str_starts_with(char const * value, char const * prefix)
{
    while (*prefix != 0) {
        if (*prefix != *value) {
            return 0;
        }

        prefix++;
        value++;
    }

    return 1;
}
