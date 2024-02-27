#ifndef PE_TOOLS_IMPORT_DIR_ENTRY_HPP
#define PE_TOOLS_IMPORT_DIR_ENTRY_HPP

#include <cinttypes>

namespace pe_tools
{

struct import_dir_entry
{
    uint32_t import_lookup_table_rva;
    uint32_t time_date_stamp;
    uint32_t forwarder_chain;
    uint32_t name_rva;
    uint32_t import_address_table_rva;

    bool is_empty() const {
        return ((0 == import_lookup_table_rva) && 
            (0 == time_date_stamp) &&
            (0 == forwarder_chain) && 
            (0 == name_rva) && 
            (0 == import_address_table_rva));
    }
};

}

#endif
