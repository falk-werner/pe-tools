#include "util/str_stack.h"
#include "util/heap.h"
#include "util/str.h"

#define STR_STACK_INITIAL_CAPACITY 16

struct str_stack * str_stack_new(void)
{
    struct str_stack * stack = heap_alloc(sizeof(struct str_stack));
    stack->data = heap_alloc(sizeof(char *) * STR_STACK_INITIAL_CAPACITY);
    stack->size = 0;
    stack->capacity = STR_STACK_INITIAL_CAPACITY;
}

void str_stack_release(struct str_stack * stack)
{
    for(int i = 0; i < stack->size; i++) {
        heap_free(stack->data[i]);        
    }
    heap_free(stack->data);
    heap_free(stack);
}

void str_stack_push(struct str_stack * stack, char * value)
{
    if (stack->capacity <= stack->size) {
        stack->capacity *= 2;
        stack->data = heap_realloc(stack->data, sizeof(char*) * stack->capacity);
    }

    stack->data[stack->size] = str_dup(value);
    stack->size++;
}

char * str_stack_pop(struct str_stack * stack)
{
    if (stack->size > 0) {
        stack->size--;
        return stack->data[stack->size];
    }

    return 0;
}

int str_stack_contains(struct str_stack * stack, char const * value)
{
    for(int i = 0; i < stack->size; i++) {
        if (str_eq(value, stack->data[i])) {
            return 1;
        }
    }

    return 0;
}
