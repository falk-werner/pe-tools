// Copyright (c) 2024 Falk Werner
// SPDX-License-Identifier: MIT

#ifndef UTIL_STR_BUILDER_H
#define UTIL_STR_BUILDER_H

struct str_builder
{
    char * data;
    unsigned int size;
    unsigned int capacity;
};

struct str_builder * str_builder_new(void);
void str_builder_release(struct str_builder * builder);
void str_builder_add_c(struct str_builder * builder, char c);
void str_builder_reset(struct str_builder * builder);


#endif
