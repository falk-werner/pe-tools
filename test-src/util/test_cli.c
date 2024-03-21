#include "alltests.h"
#include "util/cli.h"
#include "util/str.h"

int test_cli_short_flag(void)
{
    int argc = 2;
    char arg0[] = "some.app";
    char arg1[] = "-h";
    char * argv[] = { arg0, arg1 };

    struct cli_opt const opts[] = {
        {'h', "help", CLI_NO_ARG, 42},
        {0, 0, 0, 0}
    };
    int state = CLI_STATE_INIT;
    char const * out;
    int rc = cli_getopt(&state, argc, argv, opts, &out);

    if (rc != 42) {
        test_set_message("42 expected");
        return 1;
    }

    rc = cli_getopt(&state, argc, argv, opts, &out);

    if (rc != CLI_END) {
        test_set_message("CLI_END expected");
        return 1;
    }

    return 0;
}

int test_cli_long_flag(void)
{
    int argc = 2;
    char arg0[] = "some.app";
    char arg1[] = "--help";
    char * argv[] = { arg0, arg1 };

    struct cli_opt const opts[] = {
        {'h', "help", CLI_NO_ARG, 42},
        {0, 0, 0, 0}
    };
    int state = CLI_STATE_INIT;
    char const * out;
    int rc = cli_getopt(&state, argc, argv, opts, &out);

    if (rc != 42) {
        test_set_message("42 expected");
        return 1;
    }

    rc = cli_getopt(&state, argc, argv, opts, &out);

    if (rc != CLI_END) {
        test_set_message("CLI_END expected");
        return 1;
    }

    return 0;
}

int test_cli_short_opt(void)
{
    int argc = 3;
    char arg0[] = "some.app";
    char arg1[] = "-n";
    char arg2[] = "Bob";
    char * argv[] = { arg0, arg1, arg2 };

    struct cli_opt const opts[] = {
        {'h', "help", CLI_NO_ARG, 42},
        {'n', "name", CLI_REQ_ARG, 23},
        {0, 0, 0, 0}
    };
    int state = CLI_STATE_INIT;
    char const * out;
    int rc = cli_getopt(&state, argc, argv, opts, &out);

    if (rc != 23) {
        test_set_message("23 expected");
        return 1;
    }

    if (!str_eq(out, "Bob")) {
        test_set_message("Bob expected");
        return 1;
    }

    rc = cli_getopt(&state, argc, argv, opts, &out);

    if (rc != CLI_END) {
        test_set_message("CLI_END expected");
        return 1;
    }

    return 0;
}

int test_cli_long_opt(void)
{
    int argc = 3;
    char arg0[] = "some.app";
    char arg1[] = "--name";
    char arg2[] = "Bob";
    char * argv[] = { arg0, arg1, arg2 };

    struct cli_opt const opts[] = {
        {'h', "help", CLI_NO_ARG, 42},
        {'n', "name", CLI_REQ_ARG, 23},
        {0, 0, 0, 0}
    };
    int state = CLI_STATE_INIT;
    char const * out;
    int rc = cli_getopt(&state, argc, argv, opts, &out);

    if (rc != 23) {
        test_set_message("23 expected");
        return 1;
    }

    if (!str_eq(out, "Bob")) {
        test_set_message("Bob expected");
        return 1;
    }

    rc = cli_getopt(&state, argc, argv, opts, &out);

    if (rc != CLI_END) {
        test_set_message("CLI_END expected");
        return 1;
    }

    return 0;
}

int test_cli_extra_arg(void)
{
    int argc = 2;
    char arg0[] = "some.app";
    char arg1[] = "extra_arg";
    char * argv[] = { arg0, arg1 };

    struct cli_opt const opts[] = {
        {'h', "help", CLI_NO_ARG, 42},
        {'n', "name", CLI_REQ_ARG, 23},
        {0, 0, 0, 0}
    };
    int state = CLI_STATE_INIT;
    char const * out;
    int rc = cli_getopt(&state, argc, argv, opts, &out);

    if (rc != CLI_EXTRA_ARG) {
        test_set_message("CLI_EXTRA_ARG expected");
        return 1;
    }

    if (!str_eq(out, "extra_arg")) {
        test_set_message("extra_arg expected");
        return 1;
    }

    rc = cli_getopt(&state, argc, argv, opts, &out);

    if (rc != CLI_END) {
        test_set_message("CLI_END expected");
        return 1;
    }

    return 0;
}

int test_cli_empty_opts(void)
{
    int argc = 1;
    char arg0[] = "some.app";
    char * argv[] = { arg0 };

    struct cli_opt const opts[] = {
        {'h', "help", CLI_NO_ARG, 42},
        {'n', "name", CLI_REQ_ARG, 23},
        {0, 0, 0, 0}
    };
    int state = CLI_STATE_INIT;
    char const * out;
    int rc = cli_getopt(&state, argc, argv, opts, &out);

    if (rc != CLI_END) {
        test_set_message("CLI_END expected");
        return 1;
    }

    return 0;
}

int test_cli_error_missing_arg(void)
{
    int argc = 2;
    char arg0[] = "some.app";
    char arg1[] = "-n";
    char * argv[] = { arg0, arg1 };

    struct cli_opt const opts[] = {
        {'h', "help", CLI_NO_ARG, 42},
        {'n', "name", CLI_REQ_ARG, 23},
        {0, 0, 0, 0}
    };
    int state = CLI_STATE_INIT;
    char const * out;
    int rc = cli_getopt(&state, argc, argv, opts, &out);

    if (rc != CLI_ERROR) {
        test_set_message("CLI_ERROR expected");
        return 1;
    }

    return 0;
}

int test_cli_error_unknown_opt(void)
{
    int argc = 2;
    char arg0[] = "some.app";
    char arg1[] = "-u";
    char * argv[] = { arg0, arg1 };

    struct cli_opt const opts[] = {
        {'h', "help", CLI_NO_ARG, 42},
        {'n', "name", CLI_REQ_ARG, 23},
        {0, 0, 0, 0}
    };
    int state = CLI_STATE_INIT;
    char const * out;
    int rc = cli_getopt(&state, argc, argv, opts, &out);

    if (rc != CLI_ERROR) {
        test_set_message("CLI_ERROR expected");
        return 1;
    }

    return 0;
}

int test_cli_complex(void)
{
    int argc = 5;
    char arg0[] = "some.app";
    char arg1[] = "-h";
    char arg2[] = "-n";
    char arg3[] = "John";
    char arg4[] = "Doe";
    char * argv[] = { arg0, arg1, arg2, arg3, arg4 };

    struct cli_opt const opts[] = {
        {'h', "help", CLI_NO_ARG, 42},
        {'n', "name", CLI_REQ_ARG, 23},
        {0, 0, 0, 0}
    };
    int state = CLI_STATE_INIT;
    char const * out;

    int rc = cli_getopt(&state, argc, argv, opts, &out);
    if (rc != 42) {
        test_set_message("42 expected");
        return 1;
    }

    rc = cli_getopt(&state, argc, argv, opts, &out);
    if (rc != 23) {
        test_set_message("42 expected");
        return 1;
    }
    if (!str_eq(out, "John")) {
        test_set_message("John expected");
        return 1;
    }

    rc = cli_getopt(&state, argc, argv, opts, &out);
    if (rc != CLI_EXTRA_ARG) {
        test_set_message("CLI_EXTRA_ARG expected");
        return 1;
    }
    if (!str_eq(out, "Doe")) {
        test_set_message("Doe expected");
        return 1;
    }

    rc = cli_getopt(&state, argc, argv, opts, &out);
    if (rc != CLI_END) {
        test_set_message("CLI_END expected");
        return 1;
    }

    return 0;
}
