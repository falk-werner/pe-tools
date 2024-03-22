// Copyright (c) 2024 Falk Werner
// SPDX-License-Identifier: MIT

#include "util/str_builder.h"
#include "util/str.h"
#include "util/heap.h"

#define STR_BUILDER_INITIAL_CAPACITY 16

struct str_builder * str_builder_new(void)
{
    struct str_builder * builder = heap_alloc(sizeof(struct str_builder));
    builder->data = heap_alloc(STR_BUILDER_INITIAL_CAPACITY);
    builder->data[0] = '\0';
    builder->size = 1;
    builder->capacity = STR_BUILDER_INITIAL_CAPACITY;

    return builder;
}

void str_builder_release(struct str_builder * builder)
{
    heap_free(builder->data);
    heap_free(builder);
}

void str_builder_add_c(struct str_builder * builder, char c)
{
    if (builder->capacity <= builder->size) {
        builder->capacity *= 2;
        builder->data = heap_realloc(builder->data, builder->capacity);
    }

    builder->data[builder->size - 1] = c;
    builder->data[builder->size] = '\0';
    builder->size++;
}

void str_builder_reset(struct str_builder * builder)
{
    builder->data[0] = '\0';
    builder->size = 1;
}
