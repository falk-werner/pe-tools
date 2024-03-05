#include <pe_tools/pe_file.hpp>

#include <iostream>
#include <string>

using pe_tools::pe_file;

int main(int argc, char* argv[]) {
    if (argc == 2) {
        try {
            std::string const filename = argv[1];
            pe_file f(filename);

            auto const & header = f.get_coff_header();
            std::cout << "COFF header:" << std::endl;
            std::cout << "\tmachine                : 0x" << std::hex << header.machine << std::endl;
            std::cout << "\tnumber of sections     : " << std::dec << header.number_of_sections << std::endl;
            std::cout << "\ttime date stamp        : " << std::dec << header.time_date_stamp << std::endl;
            std::cout << "\tpointer to symbol table: 0x" << std::hex << header.pointer_to_symbol_table << std::endl;
            std::cout << "\tnumber of symbols      : " << std::dec << header.number_of_symbols << std::endl;
            std::cout << "\tsize of optional header: " << std::dec << header.size_of_optional_header << std::endl;
            std::cout << "\tcharacteristics        : 0x" << std::hex << header.characteristics << std::endl;
            std::cout << std::endl;

            auto const & opt_header = f.get_optional_header();
            std::cout << "Optional header:" << std::endl;
            std::cout << "\tmagic                     : 0x" << std::hex << opt_header.magic << std::endl;
            std::cout << "\tmajor linker version      : " << std::dec << (int) opt_header.major_linker_version << std::endl;
            std::cout << "\tminor linker version      : " << std::dec << (int) opt_header.minor_linker_version << std::endl;
            std::cout << "\tsize of code              : 0x" << std::hex << opt_header.size_of_code << std::endl;
            std::cout << "\tsize of initialized data  : 0x" << std::hex << opt_header.size_of_initialized_data << std::endl;
            std::cout << "\tsize of uninitialized data: 0x" << std::hex << opt_header.size_of_uninitialized_data << std::endl;
            std::cout << "\taddress of entry point    : 0x" << std::hex << opt_header.address_of_entry_point << std::endl;
            std::cout << "\tbase of code              : 0x" << std::hex << opt_header.base_of_code << std::endl;
            std::cout << "\tbase of data              : 0x" << std::hex << opt_header.base_of_data << std::endl;
            std::cout << "\timage base                : 0x" << std::hex << opt_header.image_base << std::endl;
            std::cout << "\tsection alignment         : 0x" << std::hex << opt_header.section_alignment << std::endl;
            std::cout << "\tfile alignment            : 0x" << std::hex << opt_header.file_alignment << std::endl;
            std::cout << "\tmajor OS version          : " << std::dec << opt_header.major_os_version << std::endl;
            std::cout << "\tminor OS version          : " << std::dec << opt_header.minor_os_version << std::endl;
            std::cout << "\tmajor image version       : " << std::dec << opt_header.major_image_version << std::endl;
            std::cout << "\tminor image version       : " << std::dec << opt_header.minor_image_version << std::endl;
            std::cout << "\tmajor subsystem version   : " << std::dec << opt_header.major_subsystem_version << std::endl;
            std::cout << "\tminor subsystem version   : " << std::dec << opt_header.minor_subsystem_version << std::endl;
            std::cout << "\twin32 version value       : " << std::dec << opt_header.win32_version_value << std::endl;
            std::cout << "\tsize of image             : 0x" << std::hex << opt_header.size_of_image << std::endl;
            std::cout << "\tsize of headers           : 0x" << std::hex << opt_header.size_of_headers << std::endl;
            std::cout << "\tchecksum                  : 0x" << std::hex << opt_header.checksum << std::endl;
            std::cout << "\tsubsystem                 : 0x" << std::hex << opt_header.subsystem << std::endl;
            std::cout << "\tdll characteristics       : 0x" << std::hex << opt_header.dll_characteristics << std::endl;
            std::cout << "\tsize of stack reserve     : 0x" << std::hex << opt_header.size_of_stack_reserve << std::endl;
            std::cout << "\tsize of stack commit      : 0x" << std::hex << opt_header.size_of_stack_commit << std::endl;
            std::cout << "\tsize of heap reserve      : 0x" << std::hex << opt_header.size_of_heap_reserve << std::endl;
            std::cout << "\tsize of heap commit       : 0x" << std::hex << opt_header.size_of_heap_commit << std::endl;
            std::cout << "\tloader flags              : 0x" << std::hex << opt_header.loader_flags << std::endl;
            std::cout << "\tnumber of RVA and sizes   : " << std::dec << opt_header.number_of_rva_and_sizes << std::endl;
            std::cout << std::endl;

            std::cout << "RVA and sizes:" << std::endl;
            size_t i = 0;
            for(auto const &entry: f.get_rva_and_sizes()) {
                std::cout << "\tRVA            : " << std::dec << i++ << std::endl;
                std::cout << "\tvirtual size   : 0x" << std::hex << entry.size << std::endl;
                std::cout << "\tvirtual address: 0x" << std::hex << entry.virtual_address << std::endl;
                std::cout << std::endl;
            }

            std::cout << "Section Headers:" << std::endl;
            for(auto const &section: f.get_sections()) {
                std::cout << "\tsection name           : " << section.name << std::endl;
                std::cout << "\tvirtual size           : 0x" << std::hex << section.virtual_size << std::endl;
                std::cout << "\tvirtual address        : 0x" << std::hex << section.virtual_address << std::endl;
                std::cout << "\tsize of raw data       : 0x" << std::hex << section.size_of_raw_data << std::endl;
                std::cout << "\tpointer to raw data    : 0x" << std::hex << section.pointer_to_raw_data << std::endl;
                std::cout << "\tpointer to relocations : 0x" << std::hex << section.pointer_to_relocations << std::endl;
                std::cout << "\tpointer to line numbers: 0x" << std::hex << section.pointer_to_line_numbers << std::endl;
                std::cout << "\tnumber of relocations  : " << std::dec << section.number_of_relocations << std::endl;
                std::cout << "\tnumber of line numbers : " << std::dec << section.number_of_line_numbers << std::endl;
                std::cout << "\tcharacteristcs         : 0x" << std::hex << section.characteristics << std::endl;
                std::cout << std::endl;
            }

            std::cout << "Imported DLLs:" << std::endl;
            for(auto const & dll: f.get_import_dlls()) {
                std::cout << '\t' << dll << std::endl;
            }
        }
        catch (std::exception const & ex) {
            std::cerr << "error: " << ex.what() << std::endl;
            return EXIT_FAILURE;
        }
    }
    else {
        std::cout << "usage: pe-deps <filename>" << std::endl;
    }

    return EXIT_SUCCESS;
}