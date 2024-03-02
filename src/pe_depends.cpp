#include <pe_tools/pe_file.hpp>
#include <pe_tools/dll_finder.hpp>

#include <getopt.h>

#include <iostream>
#include <string>
#include <queue>
#include <unordered_set>
#include <stdexcept>

using pe_tools::pe_file;
using pe_tools::dll_finder;

namespace
{

enum class command
{
    list,
    list_direct,
    copy,
    show_help,
};

struct context
{
    context(int argc, char* argv[])
    {
        exit_code = EXIT_SUCCESS;
        cmd = command::list;
        target = std::filesystem::current_path();

        option long_opts[] = {
            {"--list", no_argument, nullptr, 'l'},
            {"--direct", no_argument, nullptr, 'd'},
            {"--copy", no_argument, nullptr, 'c'},
            {"--help", no_argument, nullptr, 'h'},
            {"--work-dir", required_argument, nullptr, 'w'},
            {"--target", required_argument, nullptr, 't'},
            {"--path", required_argument, nullptr, 'p'},
            {"--known", required_argument, nullptr, 'k'},
            {"--no-default", no_argument, nullptr, 'n'},
            {"--file", required_argument, nullptr, 'f'},
            {nullptr, 0, nullptr, 0}
        };

        optind = 0;
        opterr = 0;

        bool done = false;
        while (!done) {
            int option_index = 0;
            int const c = getopt_long(argc, argv, "ldchC:t:p:k:nf:", long_opts, &option_index);
            switch (c) {
                case -1:
                    done = true;
                    break;
                case 'l':
                    cmd = command::list;
                    break;
                case 'd':
                    cmd = command::list_direct;
                    break;
                case 'c':
                    cmd = command::copy;
                    break;
                case 'h':
                    cmd = command::show_help;
                    break;
                case 'C':
                    std::filesystem::current_path(optarg);
                    break;
                case 't':
                    target = optarg;
                    break;
                case 'p':
                    finder.add_search_path(optarg);
                    break;
                case 'k':
                    finder.add_known_dll(optarg);
                    break;
                case 'n':
                    finder.disable_default_search_order(true);
                    break;
                case 'f':
                    app = optarg;
                    break;
                default:
                    std::cerr << "error: unknown option" << std::endl;
                    done = true;
                    exit_code = EXIT_FAILURE;
                    cmd = command::show_help;
                    break;
            }
        }

        if ((cmd != command::show_help) && (app.empty())) {
            std::cerr << "error: missing file" << std::endl;
            done = true;
            exit_code = EXIT_FAILURE;
            cmd = command::show_help;
        }
    }

    int exit_code;
    command cmd;
    std::string app;
    std::filesystem::path target;
    std::string target_directory;
    dll_finder finder;
};

void print_usage()
{
    std::cout << R"(pe-depends, (c) 2024 Falk Werner <github.com/falk-werner>
List or copy Microsoft PE file dependencies (DLLs)

Usage:
    pe-depends [--list | --direct | --copy] [-C <directory>] [-t <directory>]
               [-p <directory>] [-k <dll>] [-n] [-f <file>] [-h]

Options:
    -l, --list           - list dependencies of the PE file
    -d, --direct         - list only direct dependencies of the PE file
    -c, --copy           - copy dependencies of PE file to target directory
    -h, --help           - print this message
    -C, --work-dir <dir> - set working directory (default .)
    -t, --target <dir>   - set target directory (for copy command, default: .)
    -p, --path <dir>     - add DLL search directory (can be specified multiple times)
    -k, --known <dll>    - add known DLL (can be specified multiple times)
    -n, --no-default     - disable default search order
    -f, --file <file>    - PE file

    Known DLLs
    ----------
    A "known DLL" is a DLL that is well known by the system and therefore not
    copied to the target directory. Windows manages a list of known DLLs in
    registry (see HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\
    Session Manager\KnownDLLs).

    Use the -k option to add extra known DLLs.
    Use the -n option to ignore registry (extra known DLLs will still be used).

    Default Search Order
    --------------------
    The default search order is roughly oriented at DLL search order (see 
    https://learn.microsoft.com/en-us/windows/win32/dlls/
    dynamic-link-library-search-order). Although, some parts are ignored,
    such as DLL redirection, API sets, SxS manifests, loaded module lists,
    package manifests and the 16 bit folder. This results effectively in
    the following search order:
    - known DLLs
    - current path
    - system directory
    - windows directory
    - path variable
    - extra paths specified by -p

    Use the -n option to default search order and onyl hone extra paths.

Examples:

    List depenencies of an exe file
        pe-depends -f some.exe

    Copy dependencies of an exe file to a target directory
        pe-depends -c -f some.exe -t target
)";
}

int list(std::string const & app, dll_finder const & finder)
{
    std::unordered_set<std::string> dlls;
    std::queue<std::string> outstanding;

    {
        pe_file pe(app);
        for(auto const & dll: pe.get_import_dlls()) {
            outstanding.push(dll);
        }
    }

    while (!outstanding.empty()) {
        auto dll = outstanding.front();
        outstanding.pop();

        if (dlls.find(dll) == dlls.end()) {
            dlls.insert(dll);            
            if (finder.is_known(dll)) {
                std::cout << dll << " (known)" << std::endl;
            }
            else {
                auto path = finder.find(dll);
                if (path) {
                    std::cout << dll << " " << path.value().string() << std::endl;
                    pe_file pe( (path.value() / dll).string() );
                    for(auto const & dll: pe.get_import_dlls()) {
                        outstanding.push(dll);
                    }
                }
                else {
                    std::cout << dll << " (not found)" << std::endl;
                }
            }
        }
    }


    return EXIT_SUCCESS;
}

int list_direct(std::string const & app, dll_finder const & finder)
{
    pe_file pe(app);
    for(auto const & dll: pe.get_import_dlls()) {
        if (finder.is_known(dll)) {
            std::cout << dll << " (known)" << std::endl;
        }
        else {
            auto path = finder.find(dll);
            if (path) {
                std::cout << dll << " " << path.value().string() << std::endl; 
            }
            else {
                std::cout << dll << " (not found)" << std::endl;
            }
        }
    }

    return EXIT_SUCCESS;
}

int copy(std::string const & app, std::filesystem::path const & target, dll_finder const & finder)
{
    std::unordered_set<std::string> dlls;
    std::queue<std::string> outstanding;

    {
        pe_file pe(app);
        for(auto const & dll: pe.get_import_dlls()) {
            outstanding.push(dll);
        }
    }

    while (!outstanding.empty()) {
        auto dll = outstanding.front();
        outstanding.pop();

        if (dlls.find(dll) == dlls.end()) {
            dlls.insert(dll);            
            if (!finder.is_known(dll)) {
                auto path = finder.find(dll);
                if (path) {
                    std::filesystem::copy_file(path.value() / dll, target / dll);
                    pe_file pe( (path.value() / dll).string() );
                    for(auto const & dll: pe.get_import_dlls()) {
                        outstanding.push(dll);
                    }
                }
            }
        }
    }

    return EXIT_SUCCESS;
}

}

int main(int argc, char* argv[]) {
    try {
        context ctx(argc, argv);
        switch (ctx.cmd) {
            case command::list:
                ctx.exit_code = list(ctx.app, ctx.finder);
                break;
            case command::list_direct:
                ctx.exit_code = list_direct(ctx.app, ctx.finder);
                break;
            case command::copy:
                ctx.exit_code = copy(ctx.app, ctx.target, ctx.finder);
                break;
            case command::show_help:
                // fall-through
            default:
                print_usage();
                break;
        }
        return ctx.exit_code;
    } catch (std::exception const &ex) {
        std::cerr << "error: " << ex.what() << std::endl;
        return EXIT_FAILURE;
    } catch (...) {
        std::cerr << "fatal: unexpected error" << std::endl;
        return EXIT_FAILURE;
    }
}
