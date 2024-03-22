#ifndef PE_READER_H
#define PE_READER_H

#include <pe/coff_header.h>
#include <pe/section_header.h>
#include <pe/error.h>

#ifdef __cplusplus
extern "C"
{
#endif

struct pe_reader;

struct pe_reader *
pe_reader_from_file(char const * filename, struct pe_error * * err);

void pe_reader_release(struct pe_reader * reader);

struct pe_coff_header const *
pe_reader_get_coff_header(struct pe_reader const * reader);

struct pe_section_header const *
pe_reader_get_section_header(struct pe_reader const * reader, int i);

int pe_reader_get_number_of_import_dlls(struct pe_reader const * reader);

char const *
pe_reader_get_import_dll(struct pe_reader const * reader, int i);


#ifdef __cplusplus
}
#endif

#endif
