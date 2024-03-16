#include "pe/buffer_reader.h"

void pe_buffer_reader_init(struct pe_buffer_reader * reader, pe_u8 const * data, int size)
{
    reader->data = data;
    reader->pos = 0;
    reader->size = size;
}

pe_u8 pe_buffer_read_u8(struct pe_buffer_reader * reader)
{
    if (reader->pos < reader->size) {
        return reader->data[reader->pos++];
    }
    else {
        return 0;
    }
}

pe_u16 pe_buffer_read_u16(struct pe_buffer_reader * reader)
{
    if (reader->pos < (reader->size - 1)) {
        pe_u16 const low = reader->data[reader->pos++];
        pe_u16 const high  = reader->data[reader->pos++];
        return (high << 8) | low;
    }
    else {
        return 0;
    }
}

pe_u32 pe_buffer_read_u32(struct pe_buffer_reader * reader)
{
    if (reader->pos < (reader->size - 3)) {
        pe_u32 value = 0;
        for(int i = 0; i < 4; i++) {
            value <<= 8;
            value |= reader->data[reader->pos + 3 - i];
        }
        reader->pos += 4;
        return value;
    }
    else {
        return 0;
    }
}

pe_u64 pe_buffer_read_u64(struct pe_buffer_reader * reader)
{
    if (reader->pos < (reader->size - 7)) {
        pe_u64 value = 0;
        for(int i = 0; i < 8; i++) {
            value <<= 8;
            value |= reader->data[reader->pos + 7 - i];
        }
        reader->pos += 8;
        return value;
    }
    else {
        return 0;
    }
}

void pe_buffer_read_fixedstr(struct pe_buffer_reader * reader, char * buffer, int size)
{
    if (reader->pos < (reader->size - (size -1))) {
        for(int i = 0; i < size; i++) {
            buffer[i] = reader->data[reader->pos + i];
        }

        buffer[size] = '\0';
        reader->pos += size;
    }
    else {
        buffer[0] = '\0';
    }
}
