#ifndef PE_TOOLS_DLL_FINDER_HPP
#define PE_TOOLS_DLL_FINDER_HPP

#include <optional>
#include <filesystem>
#include <string>

namespace pe_tools
{

class dll_finder
{
public:
    dll_finder();
    std::optional<std::filesystem::path> find(std::string const & dll) const;
    bool is_known(std::string const & dll) const;
    void disable_default_search_order(bool value) const;
    void add_known_dll(std::string const & dll);
    void add_search_path(std::filesystem::path const & path);    
private:
    class detail;
    detail * d;
};

}

#endif
