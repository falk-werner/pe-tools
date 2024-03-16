#ifndef PE_DEPENDS_STRING_SET_H
#define PE_DEPENDS_STRING_SET_H

#include "string.h"

#ifndef __cplusplus
extern "C"
{
#endif

struct string_set {
    struct string * entries[];
    int size;
    int capacity;
};

struct string_set string_set_new();
void string_set_release(struct string_set set);
void string_set_add(struct string_set * set, char const * value);
int string_set_contains(struct string_set * set, char const * value);

#ifndef __cplusplus
}
#endif


#endif
