// Copyright (c) 2024 Falk Werner
// SPDX-License-Identifier: MIT

#include "pe/error.h"
#include "util/heap.h"
#include "util/str.h"

/// \file error.c
/// \brief Implementation of pe_error struct.

struct pe_error
{
    char * message;     ///< error message
};

void pe_error_set(struct pe_error * * err, char const * message)
{
    if (err != 0)
    {
        *err = heap_alloc(sizeof(struct pe_error));
        (*err)->message = str_dup(message);
    }
}

void pe_error_release(struct pe_error * err)
{
    if (0 != err)
    {
        heap_free(err->message);
        heap_free(err);
    }
}

char const * pe_error_message(struct pe_error const * err)
{
    if (0 != err)
    {
        return err->message;
    }

    return "";
}

