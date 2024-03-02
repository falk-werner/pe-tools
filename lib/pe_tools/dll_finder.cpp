#include "pe_tools/dll_finder.hpp"
#include "pe_tools/known_dlls.hpp"

#include <windows.h>

#include <cstdlib>

#include <vector>
#include <sstream>
#include <unordered_set>
#include <algorithm>

namespace
{

std::string str_tolower(std::string const & value)
{
    std::string result = value;
    std::transform(result.begin(), result.end(), result.begin(), 
        [](auto c){ return std::tolower(c); });
    return result;
}

}

namespace pe_tools
{

class dll_finder::detail
{
public:
    detail()
    : use_default_search_order(true)
    {
        for(auto const & dll: get_known_dlls()) {
            known_dlls.insert(str_tolower(dll));
        }
    }

    bool use_default_search_order;
    std::unordered_set<std::string> known_dlls;
    std::unordered_set<std::string> extra_knwon_dlls;
    std::vector<std::filesystem::path> search_paths;
};

dll_finder::dll_finder()
: d(new detail())
{

}

std::optional<std::filesystem::path> dll_finder::find(std::string const & dll) const
{
    if (is_known(dll)) { return {}; }

    auto const name = str_tolower(dll);
    std::vector<std::filesystem::path> search_paths;

    if (d->use_default_search_order) {
        search_paths.push_back(std::filesystem::current_path());

        char buffer[1024];
        if (0 != GetSystemDirectory(buffer, 1024)) {
            search_paths.push_back(buffer);
        }

        if (0 != GetWindowsDirectory(buffer, 1024)) {
            search_paths.push_back(buffer);
        }

        std::istringstream paths(std::getenv("PATH"));
        std::string path;
        while (std::getline(paths, path, ';')) {
            search_paths.push_back(path);
        }
    }

    for (auto const & path: d->search_paths) {
        search_paths.push_back(path);
    }

    for(auto const & path: search_paths) {
        if (std::filesystem::is_regular_file(path / name)) {
            return path;
        }
    }

    return {};
}

bool dll_finder::is_known(std::string const & dll) const
{
    auto const name = str_tolower(dll);

    return ((d->use_default_search_order && 
            (d->known_dlls.find(name) != d->known_dlls.end())) 
        || (d->extra_knwon_dlls.find(name) != d->extra_knwon_dlls.end()));
}

void dll_finder::disable_default_search_order(bool value) const
{
    d->use_default_search_order = !value;
}

void dll_finder::add_known_dll(std::string const & dll)
{
    d->extra_knwon_dlls.insert(str_tolower(dll));
}

void dll_finder::add_search_path(std::filesystem::path const & path)
{
    d->search_paths.push_back(path);
}


}