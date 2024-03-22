// Copyright (c) 2024 Falk Werner
// SPDX-License-Identifier: MIT

#include "pe/reader.h"
#include "pe/dll_finder.h"
#include "version.h"

#include "util/args.h"
#include "util/cli.h"
#include "util/str_stack.h"
#include "util/heap.h"
#include "util/print.h"
#include "util/str.h"

#include <windows.h>

#define BUFFER_SIZE 1024

enum command
{
    CMD_LIST,
    CMD_LIST_DIRECT,
    CMD_COPY,
    CMD_SHOW_HELP
};

struct context
{
    enum command cmd;
    int exit_code;
    char * target_dir;
    struct str_stack * dlls;
    struct pe_dll_finder * finder;
};

static void print_usage();
static void parse_args(struct context * ctx, int argc, char * argv[]);
static int real_main(int argc, char* argv[]);

void __main(void)
{
    char const * command_line = GetCommandLine();    
    struct str_stack * args = args_parse(command_line);

    int const exit_code = real_main(args->size, args->data);

    str_stack_release(args);
    ExitProcess(exit_code);
}

static void print_usage()
{
    print(
        "pe-depends V" PETOOLS_VERSION ", (c) 2024 Falk Werner <github.com/falk-werner>\n"
        "Lists and copies EXE or DLL dependencies\n"
        "\n"
        "Usage:\n"
        "  dll-finder [--list | --copy | --direct] [-n] [-w <path>] [-t <dir>] \n"
        "             [-k <DLL>] [-p <path>] <DLL>...\n"
        "\n"
        "Options:\n"
        "  -l, --list            - list dependencies recusivly (default)\n"
        "  -d, --direct          - list only direct dependencies\n"
        "  -c, --copy            - copy dependencies to target directory\n"
        "  -w, --work-dir <path> - switch work dir\n"
        "  -t, --target          - target directory for copy (default: .)\n"
        "  -k, --known <dll>     - add known DLL (can be specified multiple times)\n"
        "  -p, --path  <dll>     - add DLL search directory (can be specified multiple times)\n"
        "  -n, --no-default      - disable default search order\n"
        "\n"
        "  Known DLLs\n"
        "  ----------\n"
        "  A \"known DLL\" is a DLL that is well known by the system and therefore not\n"
        "  copied to the target directory. Windows manages a list of known DLLs in\n"
        "  registry (see HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\\n"
        "  Session Manager\\KnownDLLs).\n"
        "\n"
        "  Use the -k option to add extra known DLLs.\n"
        "  Use the -n option to ignore registry (extra known DLLs will still be used).\n"
        "\n"
        "  Default Search Order\n"
        "  --------------------\n"
        "  The default search order is roughly oriented at DLL search order (see \n"
        "  https://learn.microsoft.com/en-us/windows/win32/dlls/)\n"
        "  dynamic-link-library-search-order). Although, some parts are ignored,\n"
        "  such as DLL redirection, API sets, SxS manifests, loaded module lists,\n"
        "  package manifests and the 16 bit folder. This results effectively in\n"
        "  the following search order:\n"
        "  - known DLLs\n"
        "  - current path\n"
        "  - system directory\n"
        "  - windows directory\n"
        "  - path variable\n"
        "  - extra paths specified by -p\n"
        "\n"
        "  Use the -n option to default search order and onyl hone extra paths.\n"
    );
}

static void parse_args(struct context * ctx, int argc, char * argv[])
{
    int done = 0;
    static struct cli_opt const opts[] =
    {
        {'l', "list", CLI_NO_ARG, 'l'},
        {'d', "direct", CLI_NO_ARG, 'd'},
        {'c', "copy", CLI_NO_ARG, 'c'},
        {'w', "work-dir", CLI_REQ_ARG, 'w'},
        {'t', "target", CLI_REQ_ARG, 't'},
        {'k', "known", CLI_REQ_ARG, 'k'},
        {'p', "path", CLI_REQ_ARG, 'p'},
        {'n', "no-default", CLI_NO_ARG, 'n'},
        {'h', "help", CLI_NO_ARG, 'h'},
        {0, 0, 0, 0}
    };

    char const * arg;
    int state = CLI_STATE_INIT;
    while (!done) {
        int const value = cli_getopt(&state, argc, argv, opts, &arg);
        switch (value)
        {
            case CLI_END:
                done = 1;
                break;
            case CLI_EXTRA_ARG:
                str_stack_push(ctx->dlls, arg);
                break;
            case 'l':
                ctx->cmd = CMD_LIST;
                break;
            case 'd':
                ctx->cmd = CMD_LIST_DIRECT;
                break;
            case 'c':
                ctx->cmd = CMD_COPY;
                break;
            case 'w':
                if (0 == SetCurrentDirectory(arg)) {
                    print_error("failed to change working directory");
                    ctx->exit_code = 1;
                    ctx->cmd = CMD_SHOW_HELP;
                    done = 1;
                }
                break;
            case 't':
                heap_free(ctx->target_dir);
                ctx->target_dir = str_dup(arg);
                break;
            case 'k':
                pe_dll_finder_add_known_dll(ctx->finder, arg);
                break;
            case 'p':
                pe_dll_finder_add_search_path(ctx->finder, arg);
                break;
            case 'n':
                pe_dll_finder_disable_default_search_order(ctx->finder, 1);
                break;
            case 'h':
                ctx->cmd = CMD_SHOW_HELP;
                break;
            default:
                print_error("failed to parse command line arguments");
                ctx->exit_code = 1;
                ctx->cmd = CMD_SHOW_HELP;
                done = 1;
                break;
        }
    }

    if (ctx->dlls->size == 0) {
        ctx->cmd = CMD_SHOW_HELP;
    }
}

static void path_join(char const * dir, char const * filename, char * buffer, int buffer_size)
{
    int const dir_len = str_len(dir);
    int const name_len = str_len(filename);

    if ((dir_len + 2 + name_len) >= buffer_size) { 
        // name too lang
        buffer[0] = '\0';
        return;
    }

    for(int i = 0; i < dir_len; i++) {
        buffer[i] = dir[i];
    }
    buffer[dir_len] = '\\';
    for(int i = 0; i < name_len; i++) {
        buffer[dir_len + 1 + i] = filename[i];
    }
    buffer[dir_len + 1 + name_len] = '\0';
}

static void list(struct context * ctx, int recursive, int copy)
{
    struct str_stack * dlls = str_stack_new();
    struct str_stack * known = str_stack_new();
    struct str_stack * paths = str_stack_new();
    struct str_stack * not_found = str_stack_new();
    struct str_stack * api_sets = str_stack_new();

    while (ctx->dlls->size > 0) {
        char * filename = str_stack_pop(ctx->dlls);
        struct pe_reader * reader = pe_reader_from_file(filename, 0);
        if (reader == 0) {
            print("error: failed to open file: ");
            print(filename);
            print("\n");
            ctx->exit_code = 1;
            continue;
        }

        for(int j = 0; j < pe_reader_get_number_of_import_dlls(reader); j++) {
            char const * dll = pe_reader_get_import_dll(reader, j);

            if ((str_starts_with(dll, "api-")) || (str_starts_with(dll, "ext-"))) {
                if (!str_stack_contains(api_sets, dll)) {
                    str_stack_push(api_sets, dll);
                }
            }
            else if ((!str_stack_contains(dlls, dll)) && (!str_stack_contains(known, dll)) && 
                (!str_stack_contains(not_found, dll)) ) {
                char buffer[BUFFER_SIZE];
                int const rc = pe_dll_finder_find(ctx->finder, dll, buffer, BUFFER_SIZE);
                switch (rc)
                {
                    case PE_DLL_FOUND:
                        str_stack_push(dlls, dll);
                        str_stack_push(paths, buffer);

                        if (recursive) {
                            str_stack_push(ctx->dlls, buffer);
                        }
                        break;
                    case PE_DLL_KNOWN:
                        str_stack_push(known, dll);
                        break;
                    default:
                        str_stack_push(not_found, dll);
                        ctx->exit_code = 1;
                        break;
                }
            }
        }

        pe_reader_release(reader);
        heap_free(filename);
    }

    for(int i = 0; i < known->size; i++) {
        char const * name = known->data[i];
        print(name); print(" KNOWN\n");
    }

    for(int i = 0; i < api_sets->size; i++) {
        char const * name = api_sets->data[i];
        print(name); print(" API_SET\n");
    }

    for(int i = 0; i < dlls->size; i++) {
        char const * name = dlls->data[i];
        char const * path = paths->data[i];
        print(name); print(" FOUND "); print(path); print("\n");

        if (copy) {
            char buffer[BUFFER_SIZE];
            path_join(ctx->target_dir, name, buffer, BUFFER_SIZE);

            if (!CopyFile(path, buffer, 0)) {
                print("error: failed to copy file: "); print(path); print("\n"); 
                ctx->exit_code = 1;
            }
        }
    }

    for(int i = 0; i < not_found->size; i++) {
        char const * name = not_found->data[i];
        print(name); print(" NOT_FOUND\n");
    }

    str_stack_release(dlls);
    str_stack_release(known);
    str_stack_release(paths);
    str_stack_release(not_found);
    str_stack_release(api_sets);
}

static int real_main(int argc, char* argv[])
{
    struct context ctx;
    ctx.cmd = CMD_LIST;
    ctx.exit_code = 0;
    ctx.target_dir = str_dup(".");
    ctx.dlls = str_stack_new();
    ctx.finder = pe_dll_finder_new();

    parse_args(&ctx, argc, argv);
    switch (ctx.cmd)
    {
        case CMD_LIST:
            list(&ctx, 1, 0);
            break;
        case CMD_LIST_DIRECT:
            list(&ctx, 0, 0);
            break;
        case CMD_COPY:
            list(&ctx, 1, 1);
            break;
        case CMD_SHOW_HELP:
            // fall-through
        default:
            print_usage();
            break;
    }

    pe_dll_finder_release(ctx.finder);
    str_stack_release(ctx.dlls);
    heap_free(ctx.target_dir);
    return ctx.exit_code;
}
