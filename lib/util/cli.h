// Copyright (c) 2024 Falk Werner
// SPDX-License-Identifier: MIT

/// \file cli.h
/// \brief Interface command line parser.

#ifndef UTIL_CLI_H
#define UTIL_CLI_H

#define CLI_NO_ARG 0            ///< specifies that an option has no arguments
#define CLI_REQ_ARG 1           ///< specifies that an option has one requiered argument
#define CLI_ERROR -1            ///< return code of cli_getopt that states that an error occured
#define CLI_END 0               ///< return code of cli_getopt that states that parsing is finished
#define CLI_EXTRA_ARG 1         ///< return code of cli_getopt that states that an extra argument occured
#define CLI_STATE_INIT 0        ///< initial value of the state variable passed to cli_getopt

/// @brief Command line option.
struct cli_opt {
    char short_opt;             ///< short option (single character)
    char const * long_opt;      ///< long option
    int has_arg;                ///< CLI_NO_ARG if there is no argument, CLI_REQ_ARG if there is one argument
    int value;                  ///< return value of cli_getopt if this option is found
};

/// @brief Reads the next option or argument from the command line.
///
/// Set state to variable initialiuzed with CLI_STATE_INIT to initialize the parsing.
///
/// This functions reads the next option or argument from the command line and returns
/// a status:
/// - CLI_ERROR: if an error occured, e.g. unknown option or missing required argument
/// - CLI_END: parsing successfully finished
/// - CLI_EXTRA_ARG: value of extra arg is stored in \a out argument
/// - any other value: value of the option found
///
/// @param state State of the parser.
/// @param argc Count of command line arguments.
/// @param argv Array of commnd line arguments.
/// @param options Pointer to an array of options.
/// @param out_arg Value of the option or argument.
/// @return See description.
int cli_getopt(int * state, int argc, char * argv[], 
    struct cli_opt const * options, char const * * out_arg);

#endif
