#ifndef UTIL_ARGS_H
#define UTIL_ARGS_H

#include "util/str_stack.h"

struct str_stack * args_parse(char const * command_line);

#endif
