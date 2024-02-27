#ifndef PE_TOOLS_SECTION_HEADER_HPP
#define PE_TOOLS_SECTION_HEADER_HPP

#include <cinttypes>
#include <string>

namespace pe_tools
{

struct section_header
{
    std::string name;
    uint32_t virtual_size;
    uint32_t virtual_address;
    uint32_t size_of_raw_data;
    uint32_t pointer_to_raw_data;
    uint32_t pointer_to_relocations;
    uint32_t pointer_to_line_numbers;
    uint16_t number_of_relocations;
    uint16_t number_of_line_numbers;
    uint32_t characteristics;
};

}

#endif
