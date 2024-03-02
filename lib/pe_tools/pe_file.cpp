#include "pe_tools/pe_file.hpp"
#include "pe_tools/file_reader.hpp"
#include "pe_tools/import_dir_entry.hpp"
#include "pe_tools/image_data_directory.hpp"

#include <filesystem>
#include <stdexcept>

namespace pe_tools
{

class pe_file::detail
{
public:
    detail(std::string const & filename)
    {
        file_reader reader(filename);

        reader.seek(PE_SIGNATURE_OFFSET_OFFSET);
        auto const signature_offset = reader.u32();
        reader.seek(signature_offset);
        auto const signature = reader.u32();
        if (signature != PE_SIGNATUE) {
            throw std::runtime_error("signature mismatch");
        }
        coff_header_offset = signature_offset + 4;

        read_header(reader);
        read_optional_header(reader);
        read_section_headers(reader);
        read_import_table(reader);
    }

private:
    void read_header(file_reader & reader)
    {
        header.machine = reader.u16();
        header.number_of_sections = reader.u16();
        header.time_date_stamp = reader.u32();
        header.pointer_to_symbol_table = reader.u32();
        header.number_of_symbols = reader.u32();
        header.size_of_optional_header = reader.u16();
        header.characteristics = reader.u16();
    }

    void read_optional_header(file_reader & reader)
    {
        auto const magic = reader.u16();
        opt_header.magic = magic;
        if ((magic != 0x10b) && (magic != 0x20b)) {
            throw std::runtime_error("invalid magic");
        }

        opt_header.major_linker_version = reader.u8();
        opt_header.minor_linker_version = reader.u8();
        opt_header.size_of_code = reader.u32();
        opt_header.size_of_initialized_data = reader.u32();
        opt_header.size_of_uninitialized_data = reader.u32();
        opt_header.address_of_entry_point = reader.u32();
        opt_header.base_of_code = reader.u32();

        opt_header.base_of_data = (magic == 0x10b) ? reader.u32(): 0;

        opt_header.image_base = (magic == 0x20b) ? reader.u64() : reader.u32();
        opt_header.section_alignment = reader.u32();
        opt_header.file_alignment = reader.u32();
        opt_header.major_os_version = reader.u16();
        opt_header.minor_os_version = reader.u16();
        opt_header.major_image_version = reader.u16();
        opt_header.minor_image_version = reader.u16();
        opt_header.major_subsystem_version = reader.u16();
        opt_header.minor_subsystem_version = reader.u16();
        opt_header.win32_version_value = reader.u32();
        opt_header.size_of_image = reader.u32();
        opt_header.size_of_headers = reader.u32();
        opt_header.checksum = reader.u32();
        opt_header.subsystem = reader.u16();
        opt_header.dll_characteristics = reader.u16();
        opt_header.size_of_stack_reserve = (magic == 0x20b) ? reader.u64() : reader.u32();
        opt_header.size_of_stack_commit = (magic == 0x20b) ? reader.u64() : reader.u32();
        opt_header.size_of_heap_reserve = (magic == 0x20b) ? reader.u64() : reader.u32();
        opt_header.size_of_heap_commit = (magic == 0x20b) ? reader.u64() : reader.u32();
        opt_header.loader_flags = reader.u32();
        opt_header.number_of_rva_and_sizes = reader.u32();

        for(size_t i = 0; i < opt_header.number_of_rva_and_sizes; i++) {
            image_data_directory data_directory;

            data_directory.virtual_address = reader.u32();
            data_directory.size = reader.u32();

            data_directories.push_back(data_directory);
        }
    }

    void read_section_headers(file_reader & reader)
    {
        uint32_t const section_table_offset = coff_header_offset + COFF_HEADER_SIZE + header.size_of_optional_header;
        reader.seek(section_table_offset);
        for(size_t i = 0; i < header.number_of_sections; i++)
        {
            section_header section;

            section.name = reader.string(8);
            section.virtual_size = reader.u32();
            section.virtual_address = reader.u32();
            section.size_of_raw_data = reader.u32();
            section.pointer_to_raw_data = reader.u32();
            section.pointer_to_relocations = reader.u32();
            section.pointer_to_line_numbers = reader.u32();
            section.number_of_relocations = reader.u16();
            section.number_of_line_numbers = reader.u16();
            section.characteristics = reader.u32();

            sections.push_back(section);
        }
    }

    void read_import_table(file_reader & reader)
    {
        if ((data_directories.size() > 1) && (data_directories[1].size > 0)) {
            auto const & import_table_dir = data_directories[1];
            auto const import_dir_table_offset = rva_to_filepos(import_table_dir.virtual_address);
            reader.seek(import_dir_table_offset);

            bool done = false;
            while (!done) {
                import_dir_entry entry;
                entry.import_lookup_table_rva = reader.u32();
                entry.time_date_stamp = reader.u32();
                entry.forwarder_chain = reader.u32();
                entry.name_rva = reader.u32();
                entry.import_address_table_rva = reader.u32();
                done = entry.is_empty();
                if (!done) {
                    import_dir_table.push_back(entry);
                }
            }

            for(auto const & entry: import_dir_table) {
                reader.seek(rva_to_filepos(entry.name_rva));
                import_dlls.push_back(reader.string());
            }
        }
    }

    section_header const & get_section(uint32_t virtual_address) {
        for (auto const & section: sections) {
            if (section.virtual_size == 0) { continue; }
            if ((section.virtual_address <= virtual_address) && (virtual_address < (section.virtual_address + section.virtual_size))) {
                return section;
            }
        }

        throw std::runtime_error("section not found");
    }

    uint32_t rva_to_filepos(uint32_t rva) {
        section_header const & section = get_section(rva);
        
        return rva + section.pointer_to_raw_data - section.virtual_address ;
    }

public:
    uint32_t coff_header_offset;
    coff_header header;
    optional_header opt_header;    
    std::vector<image_data_directory> data_directories;
    std::vector<section_header> sections;
    std::vector<import_dir_entry> import_dir_table;
    std::vector<std::string> import_dlls;

};

pe_file::pe_file(std::string const & filename)
: d(new detail(filename))
{
}

pe_file::~pe_file() {
    delete d;
}


coff_header const & pe_file::get_coff_header() const
{
    return d->header;
}

optional_header const & pe_file::get_optional_header() const
{
    return d->opt_header;
}

std::vector<section_header> const &pe_file::get_sections() const
{
    return d->sections;
}

std::vector<std::string> const & pe_file::get_import_dlls() const
{
    return d->import_dlls;
}


}