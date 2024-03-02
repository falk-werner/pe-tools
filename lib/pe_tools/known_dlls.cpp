#include "pe_tools/known_dlls.hpp"
#include <windows.h>

namespace pe_tools
{

std::vector<std::string> get_known_dlls()
{
    std::vector<std::string> known_dlls;

    HKEY key;
    LSTATUS status = RegOpenKeyEx(
        HKEY_LOCAL_MACHINE,
        "SYSTEM\\CurrentControlSet\\Control\\Session Manager\\KnownDLLs",
        0,
        KEY_READ,
        &key);

    if (status == ERROR_SUCCESS) {
        constexpr size_t const max_string_lengh = 256;
        char name[max_string_lengh];
        DWORD name_len = max_string_lengh;
        DWORD type;
        char value[max_string_lengh];
        DWORD value_len = max_string_lengh;

        DWORD i = 0;
        status = RegEnumValue(key, i, name, &name_len, nullptr, &type, (LPBYTE) value, &value_len);
        while (status == ERROR_SUCCESS) {
            if (type == REG_SZ) {
                known_dlls.push_back(value);
            }
            i++;
            name_len = max_string_lengh;
            value_len = max_string_lengh;
            status = RegEnumValue(key, i, name, &name_len, nullptr, &type, (LPBYTE) value, &value_len);
        }
        RegCloseKey(key);
    }


    return known_dlls;
}

}