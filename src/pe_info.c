#include "pe/reader.h"
#include "version.h"

#include "util/print.h"
#include "util/args.h"
#include "util/str.h"

#include <windows.h>

static void print_usage(void);
static int real_main(int argc, char * argv[]);

void __main(void)
{
    char const * command_line = GetCommandLine();
    struct str_stack * args = args_parse(command_line);

    int const exit_code = real_main(args->size, args->data);

    str_stack_release(args);
    ExitProcess(exit_code);
}

static void print_usage(void)
{
    print(
        "pe-info V" PETOOLS_VERSION ", (c) 2024 Falk Werner <github.com/falk-werner>\n"
        "Print Microsoft EXE and DLL file information\n"
        "\n"
        "Usage:\n"
        "  pe-info <filename> ...\n"
    );
}

static int real_main(int argc, char * argv[])
{
    int exit_code = 0;

    if ((argc > 1) && (!str_eq(argv[1], "--help")) && (!str_eq(argv[1], "-h"))) {
        for (int i = 1; i < argc; i++) {
            char const * filename = argv[i];
            print("File: "); print(filename); print("\n");

            struct pe_error * err;
            struct pe_reader * reader = pe_reader_from_file(filename, &err);
            if (reader != 0)
            {
                struct pe_coff_header const * header = pe_reader_get_coff_header(reader);

                print("COFF Header:\n");
                print("  machine                   : 0x"); print_hex(header->machine); print("\n");
                print("  number of sections        : "); print_u(header->number_of_sections); print("\n");
                print("  time data stamp           : "); print_u(header->time_date_stamp); print("\n");
                print("  pointer to symbol table   : 0x"); print_hex(header->pointer_to_symbol_table); print("\n");
                print("  number of symbols         : "); print_u(header->number_of_symbols); print("\n");
                print("  size of optional header   : "); print_u(header->size_of_optional_header); print("\n");
                print("  characteristics           : 0x"); print_hex(header->characteristcs); print("\n");

                print("  magic                     : 0x"); print_hex(header->magic); print("\n");
                print("  major linker version      : "); print_u(header->major_linker_version); print("\n");
                print("  minor linker version      : "); print_u(header->minor_linker_version); print("\n");
                print("  size of code              : 0x"); print_hex(header->size_of_code); print("\n");
                print("  size of initialized data  : 0x"); print_hex(header->size_of_initialized_data); print("\n");
                print("  size_of uninitialized data: 0x"); print_hex(header->size_of_uninitialized_data); print("\n");
                print("  address of entry point    : 0x"); print_hex(header->address_of_entry_point); print("\n");
                print("  base of code              : 0x"); print_hex(header->base_of_code); print("\n");

                print("  base of data              : 0x"); print_hex(header->base_of_data); print("\n");

                print("  image base                : 0x"); print_hex(header->image_base); print("\n");
                print("  section alignment         : 0x"); print_hex(header->section_alignment); print("\n");
                print("  file alignment            : 0x"); print_hex(header->file_alignment); print("\n");
                print("  major os version          : "); print_u(header->major_os_version); print("\n");
                print("  minor os version          : "); print_u(header->minor_os_version); print("\n");
                print("  major image version       : "); print_u(header->major_image_version); print("\n");
                print("  minor image version       : "); print_u(header->minor_image_version); print("\n");
                print("  major subsystem version   : "); print_u(header->major_subsystem_version); print("\n");
                print("  minor subsystem version   : "); print_u(header->minor_subsystem_version); print("\n");
                print("  win32 version value       : "); print_u(header->win32_version_value); print("\n");
                print("  size of image             : 0x"); print_hex(header->size_of_image); print("\n");
                print("  size of headers           : 0x"); print_hex(header->size_of_headers); print("\n");
                print("  checksum                  : 0x"); print_hex(header->checksum); print("\n");
                print("  subsystem                 : "); print_u(header->subsystem); print("\n");
                print("  dll characteristics       : 0x"); print_hex(header->dll_characteristics); print("\n");
                print("  size of stack reserve     : 0x"); print_hex(header->size_of_stack_reserve); print("\n");
                print("  size of stack commit      : 0x"); print_hex(header->size_of_stack_commit); print("\n");
                print("  size of heap reserve      : 0x"); print_hex(header->size_of_heap_reserve); print("\n");
                print("  size of heap commit       : 0x"); print_hex(header->size_of_heap_commit); print("\n");
                print("  loader flags              : 0x"); print_hex(header->loader_flags); print("\n");
                print("  number of rva and sizes   : "); print_u(header->number_of_rva_and_sizes); print("\n");

                print("  Export Table              :\n");
                print("    virtual address         : 0x"); print_hex(header->export_table.virtual_address); print("\n");
                print("    size                    : 0x"); print_hex(header->export_table.size); print("\n");
                print("  Import Table              :\n");
                print("    virtual address         : 0x"); print_hex(header->import_table.virtual_address); print("\n");
                print("    size                    : 0x"); print_hex(header->import_table.size); print("\n");
                print("  Resource Table            :\n");
                print("    virtual address         : 0x"); print_hex(header->resource_table.virtual_address); print("\n");
                print("    size                    : 0x"); print_hex(header->resource_table.size); print("\n");
                print("  Exception Table           :\n");
                print("    virtual address         : 0x"); print_hex(header->exception_table.virtual_address); print("\n");
                print("    size                    : 0x"); print_hex(header->exception_table.size); print("\n");
                print("  Certificate Table         :\n");
                print("    virtual address         : 0x"); print_hex(header->certificate_table.virtual_address); print("\n");
                print("    size                    : 0x"); print_hex(header->certificate_table.size); print("\n");
                print("  Base Relocation Table     :\n");
                print("    virtual address         : 0x"); print_hex(header->base_relocation_table.virtual_address); print("\n");
                print("    size                    : 0x"); print_hex(header->base_relocation_table.size); print("\n");
                print("  Debug                     :\n");
                print("    virtual address         : 0x"); print_hex(header->debug.virtual_address); print("\n");
                print("    size                    : 0x"); print_hex(header->debug.size); print("\n");
                print("  Archicture                :\n");
                print("    virtual address         : 0x"); print_hex(header->architecture.virtual_address); print("\n");
                print("    size                    : 0x"); print_hex(header->architecture.size); print("\n");
                print("  Global Ptr                :\n");
                print("    virtual address         : 0x"); print_hex(header->global_ptr.virtual_address); print("\n");
                print("    size                    : 0x"); print_hex(header->global_ptr.size); print("\n");
                print("  TLS Table                 :\n");
                print("    virtual address         : 0x"); print_hex(header->tls_table.virtual_address); print("\n");
                print("    size                    : 0x"); print_hex(header->tls_table.size); print("\n");
                print("  Load Config Table         :\n");
                print("    virtual address         : 0x"); print_hex(header->load_config_table.virtual_address); print("\n");
                print("    size                    : 0x"); print_hex(header->load_config_table.size); print("\n");
                print("  Bound Import              :\n");
                print("    virtual address         : 0x"); print_hex(header->bound_import.virtual_address); print("\n");
                print("    size                    : 0x"); print_hex(header->bound_import.size); print("\n");
                print("  IAT Table                 :\n");
                print("    virtual address         : 0x"); print_hex(header->iat.virtual_address); print("\n");
                print("    size                    : 0x"); print_hex(header->iat.size); print("\n");
                print("  Deplay Import Descriptor  :\n");
                print("    virtual address         : 0x"); print_hex(header->deplay_import_descriptor.virtual_address); print("\n");
                print("    size                    : 0x"); print_hex(header->deplay_import_descriptor.size); print("\n");
                print("  CLR Runtime Header        :\n");
                print("    virtual address         : 0x"); print_hex(header->clr_runtime_header.virtual_address); print("\n");
                print("    size                    : 0x"); print_hex(header->clr_runtime_header.size); print("\n");

                print("Section Headers:\n");
                for(int i = 0; i < header->number_of_sections; i++) {
                    struct pe_section_header const * section = pe_reader_get_section_header(reader, i);
                    if (section != 0) {
                        print("  Section: #"); print_u(i); print("\n");
                        print("    name                   : "); print(section->name); print("\n");
                        print("    virtual size           : 0x"); print_hex(section->virtual_size); print("\n");
                        print("    virtual address        : 0x"); print_hex(section->virtual_address); print("\n");
                        print("    size of raw data       : 0x"); print_hex(section->size_of_raw_data); print("\n");
                        print("    pointer to raw data    : 0x"); print_hex(section->pointer_to_raw_data); print("\n");
                        print("    pointer to relocations : 0x"); print_hex(section->pointer_to_relocations); print("\n");
                        print("    pointer to line numbers: 0x"); print_hex(section->pointer_to_line_numbers); print("\n");
                        print("    number of relocations  : "); print_hex(section->number_of_relocations); print("\n");
                        print("    number of line numbers : "); print_hex(section->number_of_line_numbers); print("\n");
                        print("    characterics           : 0x"); print_hex(section->characteristcs); print("\n");
                    }
                }

                print("Import DLLs:\n");
                for(int i = 0; i < pe_reader_get_number_of_import_dlls(reader); i++) {
                    print("  "); print(pe_reader_get_import_dll(reader, i)); print("\n");
                }

                print("\n");
                pe_reader_release(reader);
            }
            else
            {
                exit_code = 1;
                print_error(pe_error_message(err));
                pe_error_release(err);
            }
        }
    }
    else {
        print_usage();
    }

    return exit_code;
}
