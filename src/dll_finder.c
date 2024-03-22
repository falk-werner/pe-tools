#include "pe/dll_finder.h"

#include "util/args.h"
#include "util/cli.h"
#include "util/str_stack.h"
#include "util/print.h"

#include <windows.h>

#define BUFFER_SIZE 1024

struct context
{
    int exit_code;
    int show_help;
    struct str_stack * dlls;
    struct pe_dll_finder * finder;
};

static void print_usage();
static int real_main(int argc, char* argv[]);
static void parse_args(struct context * ctx, int argc, char* argv[]);

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
        "dll-finder, (c) 2024 Falk Werner <github.com/falk-werner>\n"
        "Find DLLs\n"
        "\n"
        "Usage:\n"
        "  dll-finder [-n] [-w <path>] [-k <DLL>] [-p <path>] <DLL>...\n"
        "\n"
        "Options:\n"
        "  -w, --work-dir <path> - switch work dir\n"
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

static void parse_args(struct context * ctx, int argc, char* argv[])
{
    int done = 0;
    static struct cli_opt const opts[] =
    {
        {'w', "work-dir", CLI_REQ_ARG, 'w'},
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
            case 'w':
                if (0 == SetCurrentDirectory(arg)) {
                    print_error("failed to change working directory");
                    ctx->exit_code = 1;
                    ctx->show_help = 1;
                    done = 1;
                }
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
                ctx->show_help = 1;
                break;
            default:
                print_error("failed to parse command line arguments");
                ctx->exit_code = 1;
                ctx->show_help = 1;
                done = 1;
                break;
        }
    }

    if (ctx->dlls->size == 0) {
        ctx->show_help = 1;
    }
}

static int real_main(int argc, char* argv[])
{
    struct context ctx;
    ctx.exit_code = 0;
    ctx.show_help = 0;
    ctx.dlls = str_stack_new();
    ctx.finder = pe_dll_finder_new();

    parse_args(&ctx, argc, argv);
    if (!ctx.show_help) {
        for(int i = 0; i < ctx.dlls->size; i++) {
            char buffer[BUFFER_SIZE];
            char const * dll = ctx.dlls->data[i];

            int const result = pe_dll_finder_find(ctx.finder, dll, buffer, BUFFER_SIZE);
            switch (result)
            {
                case PE_DLL_FOUND:
                    print(buffer);
                    print("\n");
                    break;
                case PE_DLL_KNOWN:
                    print("KNOWN\n");
                    break;
                case PE_DLL_NOT_FOUND:
                    // fall through
                default:
                    print("NOT_FOUND\n");
                    ctx.exit_code = 1;
                    break;
            }
        }
    }
    else {
        print_usage();
    }

    pe_dll_finder_release(ctx.finder);
    str_stack_release(ctx.dlls);
    return ctx.exit_code;
}
