// Copyright (c) 2024 Falk Werner
// SPDX-License-Identifier: MIT

/// \file cli.c
/// \brief Implementation of command line parser.

#include "util/cli.h"
#include "util/str.h"

int cli_getopt(int * state, int argc, char * argv[], 
    struct cli_opt const * options, char const * * out_arg)
{
    if (*state < 1) {
       *state = 1;
    }

    if (*state >= argc) {
        return CLI_END;
    }

    char const * arg = argv[*state];
    (*state)++;
    if (arg[0] == '-') {
        int short_opt = (arg[1] != '-');

        for(int i = 0; options[i].value != 0; i++) {
            struct cli_opt const * opt = &(options[i]);
            if (((short_opt) && (arg[1] == opt->short_opt)) ||
                ((!short_opt) && (str_eq(&(arg[2]), opt->long_opt)))) {

                if (opt->has_arg == CLI_REQ_ARG) {
                    if (*state < argc) {
                        *out_arg = argv[*state];
                        (*state)++;
                    }
                    else {
                        return CLI_ERROR;
                    }
                }
                return opt->value;
            }

        }

        return CLI_ERROR;
    }

    *out_arg = arg;
    return CLI_EXTRA_ARG;
}