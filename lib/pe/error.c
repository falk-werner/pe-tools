#include "pe/error_intern.h"
#include "util/heap.h"
#include "util/str.h"

struct pe_error
{
    char * message;
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

