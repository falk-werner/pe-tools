// Copyright (c) 2024 Falk Werner
// SPDX-License-Identifier: MIT

#ifndef UTIL_CLI_H
#define UTIL_CLI_H

#define CLI_NO_ARG 0
#define CLI_REQ_ARG 1
#define CLI_ERROR -1
#define CLI_END 0
#define CLI_EXTRA_ARG 1
#define CLI_STATE_INIT 0

struct cli_opt {
    char short_opt;
    char const * long_opt;
    int has_arg;
    int value;
};

struct cli;

int cli_getopt(int * state, int argc, char * argv[], 
    struct cli_opt const * options, char const * * out_arg);

#endif
