#include "pe_tools/file_reader.hpp"

#include <stdexcept>

namespace pe_tools
{

file_reader::file_reader(std::string const & filename)
: file(filename, std::ios_base::in | std::ios_base::binary)
{
    if (!file) {
        throw std::runtime_error("failed to open file");
    }
}

void file_reader::seek(std::iostream::pos_type pos)
{
    file.seekg(pos);
    if (!file) {
        throw std::runtime_error("failed to seek file");
    }
}

void file_reader::skip(std::iostream::pos_type count)
{
    file.seekg(count, std::ios_base::cur);
    if (!file) {
        throw std::runtime_error("failed to seek file");
    }
}

uint8_t file_reader::u8()
{
    char buffer;
    read(&buffer, 1);

    return ((uint8_t) buffer) & 0xff;
}

uint16_t file_reader::u16()
{
    char buffer[2];
    read(buffer, 2);

    uint16_t const low = ((uint16_t) buffer[0]) & 0xff;
    uint16_t const high = ((uint16_t) buffer[1]) & 0xff;

    return (high << 8) | low;
}

uint32_t file_reader::u32()
{
    char buffer[4];
    read(buffer, 4);

    uint32_t value = 0;
    for(size_t i = 0; i < 4; i++) {
        value <<= 8;
        value |= ((uint32_t) buffer[3 - i]) & 0xff;
    }

    return value;
}

uint64_t file_reader::u64()
{
    char buffer[8];
    read(buffer, 8);

    uint64_t value = 0;
    for(size_t i = 0; i < 4; i++) {
        value <<= 8;
        value |= ((uint32_t) buffer[7 - i]) & 0xff;
    }

    return value;
}

std::string file_reader::string()
{
    std::string value;

    char buffer;
    read(&buffer, 1);
    while (buffer != '\0') {
        value += buffer;
        read(&buffer, 1);
    }

    return value;
}

std::string file_reader::string(size_t size)
{
    char buffer[size];
    read(buffer, size);

    return std::string(buffer, size);
}

void file_reader::read(char * buffer, size_t buffer_size)
{
    file.read(buffer, buffer_size);
    if (!file) {
        throw std::runtime_error("failed to read file");
    }
}

}