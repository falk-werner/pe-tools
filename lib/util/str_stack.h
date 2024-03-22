#ifndef UTIL_STR_STACK_H
#define UTIL_STR_STACK_H

struct str_stack
{
    char * * data;
    unsigned int size;
    unsigned int capacity;
};

struct str_stack * str_stack_new(void);

void str_stack_release(struct str_stack * stack);

void str_stack_push(struct str_stack * stack, char const * value);

char * str_stack_pop(struct str_stack * stack);

int str_stack_contains(struct str_stack * stack, char const * value);


#endif
