#include "util/args.h"
#include "util/str_builder.h"
#include "util/panic.h"

#define READ_WHITESPACE 0
#define READ_NORMAL_ARG 1
#define READ_ESACPED_ARG 2

static int is_space(char const c)
{
    return ((c == ' ') || (c == '\t') || (c == '\n') || (c == '\r') || (c == '\f')); 
}

struct str_stack * args_parse(char const * command_line)
{
    struct str_stack * args = str_stack_new();
    struct str_builder * builder = str_builder_new();

    int pos = 0;
    int state = READ_WHITESPACE;
    while (command_line[pos] != '\0') {
        char const c = command_line[pos];
        pos++;
        switch (state)
        {
            case READ_WHITESPACE:
                if (c == '\"') {
                    state = READ_ESACPED_ARG;
                }
                else if (!is_space(c)) {
                    str_builder_add_c(builder, c);
                    state = READ_NORMAL_ARG;
                }
                break;
            case READ_NORMAL_ARG:
                if (!is_space(c)) {
                    str_builder_add_c(builder, c);                    
                }
                else {
                    str_stack_push(args, builder->data);
                    str_builder_reset(builder);
                    state = READ_WHITESPACE;
                }
                break;
            case READ_ESACPED_ARG:
                if (c == '\"') {
                    str_stack_push(args, builder->data);
                    str_builder_reset(builder);
                    state = READ_WHITESPACE;
                }
                else if ((c == '\'') && (command_line[pos] == '\"')) {
                    str_builder_add_c(builder, '\"');
                    pos++;
                }
                else {
                    str_builder_add_c(builder, c);
                }
                break;
            default:
                panic("args_get: dead code reached");
                break;
        }
    }

    if (builder->size > 1) {
        str_stack_push(args, builder->data);
    }

    str_builder_release(builder);
    return args;
}
