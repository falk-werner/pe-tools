#ifndef PE_TOOLS_FILE_READER_HPP
#define PE_TOOLS_FILE_READER_HPP

#include <cinttypes>
#include <fstream>
#include <string>

namespace pe_tools
{

class file_reader
{
public:
    file_reader(std::string const & filename);
    void seek(std::iostream::pos_type pos);
    void skip(std::iostream::pos_type count);
    uint8_t u8();
    uint16_t u16();
    uint32_t u32();
    uint64_t u64();
    std::string string();
    std::string string(size_t size);

private:
    void read(char * buffer, size_t buffer_size);

    std::ifstream file;
};

}

#endif
