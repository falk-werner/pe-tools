#ifndef PE_TOOLS_PE_FILE_HPP
#define PE_TOOLS_PE_FILE_HPP

#include <pe_tools/coff_header.hpp>
#include <pe_tools/optional_header.hpp>
#include <pe_tools/section_header.hpp>
#include <pe_tools/image_data_directory.hpp>

#include <string>
#include <vector>
#include <memory>

namespace pe_tools
{

constexpr size_t const PE_SIGNATURE_OFFSET_OFFSET = 0x3c;
constexpr uint32_t const PE_SIGNATUE = 0x4550;
constexpr uint32_t const COFF_HEADER_SIZE = 20;

class pe_file
{
    pe_file(pe_file const &) = delete;
    pe_file& operator=(pe_file const &) = delete;
    pe_file(pe_file &&) = delete;
    pe_file& operator=(pe_file &&) = delete;
public:
    explicit pe_file(std::string const & filename);
    virtual ~pe_file();

    coff_header const & get_coff_header() const;
    optional_header const & get_optional_header() const;

    std::vector<image_data_directory> const & get_rva_and_sizes() const;
    std::vector<section_header> const & get_sections() const;
    std::vector<std::string> const & get_import_dlls() const;    

private:
    class detail;
    detail * d;
};

}

#endif
