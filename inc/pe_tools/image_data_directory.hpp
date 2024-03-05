#ifndef PE_TOOLS_IMAGE_DATA_DIRECTORY_HPP
#define PE_TOOLS_IMAGE_DATA_DIRECTORY_HPP

#include <cinttypes>

namespace pe_tools
{

struct image_data_directory {
    uint32_t virtual_address;
    uint32_t size;
};


}

#endif
