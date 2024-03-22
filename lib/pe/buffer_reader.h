// Copyright (c) 2024 Falk Werner
// SPDX-License-Identifier: MIT

#ifndef PE_BUFFER_READER_H
#define PE_BUFFER_READER_H

#include "pe/types.h"

struct pe_buffer_reader
{
    pe_u8 const * data;
    int pos;
    int size;
};

void pe_buffer_reader_init(struct pe_buffer_reader * reader, pe_u8 const * data, int size);

pe_u8 pe_buffer_read_u8(struct pe_buffer_reader * reader);
pe_u16 pe_buffer_read_u16(struct pe_buffer_reader * reader);
pe_u32 pe_buffer_read_u32(struct pe_buffer_reader * reader);
pe_u64 pe_buffer_read_u64(struct pe_buffer_reader * reader);
void pe_buffer_read_fixedstr(struct pe_buffer_reader * reader, char * buffer, int size);


#endif
