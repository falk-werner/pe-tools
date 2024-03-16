#include "alltests.h"
#include "pe/buffer_reader.h"

int test_buffer_reader_read_u8(void)
{
    pe_u8 data[] = { 0, 42, 13, 37 };
    struct pe_buffer_reader reader;
    pe_buffer_reader_init(&reader, data, 4);

    if (0 != pe_buffer_read_u8(&reader)) {
        test_set_message("0 expected");
        return 1;
    }

    if (42 != pe_buffer_read_u8(&reader)) {
        test_set_message("42 expected");
        return 1;
    }

    if (13 != pe_buffer_read_u8(&reader)) {
        test_set_message("13 expected");
        return 1;
    }

    if (37 != pe_buffer_read_u8(&reader)) {
        test_set_message("37 expected");
        return 1;
    }

    if (0 != pe_buffer_read_u8(&reader)) {
        test_set_message("0 expected");
        return 1;
    }

    return 0;
}

int test_buffer_reader_read_u16(void)
{
    pe_u8 data[] = { 0x34, 0x12, 0x78, 0x56 };
    struct pe_buffer_reader reader;
    pe_buffer_reader_init(&reader, data, 4);

    if (0x1234 != pe_buffer_read_u16(&reader)) {
        test_set_message("0x1234 expected");
        return 1;
    }

    if (0x5678 != pe_buffer_read_u16(&reader)) {
        test_set_message("0x5678 expected");
        return 1;
    }

    if (0 != pe_buffer_read_u16(&reader)) {
        test_set_message("0 expected");
        return 1;
    }

    return 0;
}

int test_buffer_reader_read_u32(void)
{
    pe_u8 data[] = { 0x78, 0x56, 0x34, 0x12 };
    struct pe_buffer_reader reader;
    pe_buffer_reader_init(&reader, data, 4);

    if (0x12345678 != pe_buffer_read_u32(&reader)) {
        test_set_message("0x12345678 expected");
        return 1;
    }

    if (0 != pe_buffer_read_u32(&reader)) {
        test_set_message("0 expected");
        return 1;
    }

    return 0;
}

int test_buffer_reader_read_u64(void)
{
    pe_u8 data[] = { 0xef, 0xcd, 0xab, 0x89, 0x67, 0x45, 0x23, 0x01 };
    struct pe_buffer_reader reader;
    pe_buffer_reader_init(&reader, data, 8);

    if (0x0123456789abcdef != pe_buffer_read_u64(&reader)) {
        test_set_message("0x0123456789abcdef expected");
        return 1;
    }

    if (0 != pe_buffer_read_u64(&reader)) {
        test_set_message("0 expected");
        return 1;
    }

    return 0;
}
